#include "string.h"

#ifdef __BENCH_AOLC__
#include <aolc/_test_string.h>
#endif

#include <benchmark/benchmark.h>

#include <string>
#include <random>
#include <array>

static void BM_strncpy_NBytes(benchmark::State& state, size_t len, size_t src_off, size_t dst_off) {
    size_t src_buffer_len = len + src_off;
    size_t dst_buffer_len = len + dst_off;

    char* src_buffer = new char[src_buffer_len];
    char* dst_buffer = new char[dst_buffer_len];
    std::random_device rd;
    std::uniform_int_distribution<char> char_dist(1, 127); 
    #pragma omp parallel for
    for (size_t i = 0; i < len - 1; i++) {
        src_buffer[i + src_off] = char_dist(rd);
        dst_buffer[i + dst_off] = char_dist(rd);
    }
    src_buffer[src_buffer_len - 1] = '\0';

    char* src = src_buffer + src_off;
    char* dst = src_buffer + dst_off;
    for (auto _ : state) {
            // Only the contents of this loop get benchmarked
#ifdef __BENCH_AOLC__
            benchmark::DoNotOptimize(_strncpy(dst, src, len));
#elif  __BENCH_GLIBC__
            benchmark::DoNotOptimize(strncpy(dst, src, len));
#elif  __BENCH_MUSL__
            __builtin_unreachable();
#else
            __builtin_unreachable();
#endif
            benchmark::ClobberMemory();
    }

    delete[](src_buffer);
    delete[](dst_buffer);
}

int main(int argc, char** argv) {
    std::string str_base = "strncpy-Len";
    std::array<int, 13> test_lengths = {4, 8, 16, 32, 64, 256, 4096, 16384, 1, 10, 100, 1000, 10000};
    char test_char = '~';
    for (auto len : test_lengths) {
        benchmark::RegisterBenchmark((str_base + std::to_string(len)).c_str(), BM_strncpy_NBytes, len, 0, 0);
    }
    benchmark::Initialize(&argc, argv);
    benchmark::RunSpecifiedBenchmarks();
}
