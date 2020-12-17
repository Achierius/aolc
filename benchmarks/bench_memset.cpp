#include "string.h"

#ifdef __BENCH_AOLC__
#include <aolc/_test_string.h>
#endif

#include <benchmark/benchmark.h>

#include <string>
#include <random>
#include <array>

static void BM_Memset_NBytes(benchmark::State& state, char chr, size_t len, size_t off) {
    size_t buffer_len = len + off;
    size_t buffer_off = off;
    size_t str_len = len;

    char* buffer = new char[buffer_len];
    std::random_device rd;
    std::uniform_int_distribution<char> char_dist(1, 127); 
    #pragma omp parallel for
    for (size_t i = buffer_off; i < buffer_len - 1; i++) {
        buffer[i] = char_dist(rd);
    }

    char* test_str = buffer + buffer_off;
    for (auto _ : state) {
            // Only the contents of this loop get benchmarked
#ifdef __BENCH_AOLC__
            benchmark::DoNotOptimize(_memset(test_str, chr, len));
#elif  __BENCH_GLIBC__
            benchmark::DoNotOptimize(memset(test_str, chr, len));
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
    std::string str_base = "Memset-Len";
    std::array<int, 13> test_lengths = {4, 8, 16, 32, 64, 256, 4096, 16384, 1, 10, 100, 1000, 10000};
    char test_char = '~';
    for (auto len : test_lengths) {
        benchmark::RegisterBenchmark((str_base + std::to_string(len)).c_str(), BM_Memset_NBytes, test_char, len, 0);
    }
    benchmark::Initialize(&argc, argv);
    benchmark::RunSpecifiedBenchmarks();
}
