#include "string.h"

#ifdef __BENCH_AOLC__
#include <aolc/_test_string.h>
#endif

#include <benchmark/benchmark.h>

#include <random>

template<size_t Len, size_t Off>
static void BM_Strlen_NBytes(benchmark::State& state) {
    size_t buffer_len = Len + Off;
    size_t buffer_off = Off;
    size_t str_len = Len;

    // We fill the buffer with random bytes to expose any slowdowns that might
    // occur due to weird byte patterns (e.g. 0x81818181 for certain strlen
    // implementations which rely on a faster-but-leakier bit hack procedure)
    char* buffer = new char[buffer_len];
    std::random_device rd;
    //  If we want to only scan printable strings:
    ////std::uniform_int_distribution<char> char_dist(' ', '~'); 
    std::uniform_int_distribution<char> char_dist(1, 127); 
    for (size_t i = buffer_off; i < buffer_len - 1; i++) {
        buffer[i] = char_dist(rd);
    }
    buffer[buffer_len - 1] = '\0';

    char* test_str = buffer + buffer_off;
    volatile int unused = 0; // W/o this, gcc removes the strlen even at -O0; the assignment overhead is ~0.1ns
    for (auto _ : state) {
            // Only the contents of this loop get benchmarked
#ifdef __BENCH_AOLC__
            benchmark::DoNotOptimize(_strlen(test_str));
#elif  __BENCH_GLIBC__
            benchmark::DoNotOptimize(strlen(test_str));
#elif  __BENCH_MUSL__
            __builtin_unreachable();
#else
            __builtin_unreachable();
#endif
            benchmark::ClobberMemory();
    }

    delete[](buffer);
}

#define STRLEN_BENCHMARK(X) static void (*Strlen_Len##X)(benchmark::State& state) = &BM_Strlen_NBytes<X, 0>; \
                            BENCHMARK(Strlen_Len##X);

#define STRLEN_BENCHMARK_OFF(X, Y) static void (*Strlen_Len##X##_Off##Y)(benchmark::State& state) = &BM_Strlen_NBytes<X, Y>; \
                                   BENCHMARK(Strlen_Len##X##_Off##Y);

STRLEN_BENCHMARK(4);
STRLEN_BENCHMARK(8);
STRLEN_BENCHMARK(16);
STRLEN_BENCHMARK(32);
STRLEN_BENCHMARK(64);
STRLEN_BENCHMARK(256);
STRLEN_BENCHMARK(4096);
STRLEN_BENCHMARK(16384);

STRLEN_BENCHMARK(1);
STRLEN_BENCHMARK(10);
STRLEN_BENCHMARK(100);
STRLEN_BENCHMARK(1000);
STRLEN_BENCHMARK(10000);

BENCHMARK_MAIN();
