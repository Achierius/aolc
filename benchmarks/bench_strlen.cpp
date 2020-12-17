#include "string.h"

#ifdef __BENCH_AOLC__
#include <aolc/_test_string.h>
#endif

#include <benchmark/benchmark.h>

#include <string>
#include <random>
#include <array>

static void BM_Strlen_NBytes(benchmark::State& state, size_t len, size_t off) {
    size_t buffer_len = len + off;
    size_t buffer_off = off;
    size_t str_len = len;

    // We fill the buffer with random bytes to expose any slowdowns that might
    // occur due to weird byte patterns (e.g. 0x81818181 for certain strlen
    // implementations which rely on a faster-but-leakier bit hack procedure)
    char* buffer = new char[buffer_len];
    std::random_device rd;
    //  If we want to only scan printable strings:
    ////std::uniform_int_distribution<char> char_dist(' ', '~'); 
    std::uniform_int_distribution<char> char_dist(1, 127); 
    #pragma omp parallel for
    for (size_t i = buffer_off; i < buffer_len - 1; i++) {
        buffer[i] = char_dist(rd);
    }
    buffer[buffer_len - 1] = '\0';

    char* test_str = buffer + buffer_off;
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

int main(int argc, char** argv) {
    std::string str_base = "Strlen-Len";
    std::array<int, 13> test_lengths = {4, 8, 16, 32, 64, 256, 4096, 16384, 1, 10, 100, 1000, 10000};
    for (auto len : test_lengths) {
        benchmark::RegisterBenchmark((str_base + std::to_string(len)).c_str(), BM_Strlen_NBytes, len, 0);
    }
    benchmark::Initialize(&argc, argv);
    benchmark::RunSpecifiedBenchmarks();
}
