#include "string.h"

#ifdef __BENCH_AOLC__
#include <aolc/_test_string.h>
#endif

#include <benchmark/benchmark.h>

#include <string>
#include <random>
#include <array>
#include <functional>

static void BM_Strlen_NBytes(benchmark::State& state, size_t len, size_t off) {
    size_t buffer_len = len + off;
    size_t buffer_off = off;
    size_t str_len = len;

    // We fill the buffer with random bytes to expose any slowdowns that might
    // occur due to weird byte patterns (e.g. 0x81818181 for certain strlen
    // implementations which rely on a faster-but-leakier bit hack procedure)
    char* buffer = new char[buffer_len];
    std::random_device rd;
    std::mt19937 gen(std::hash<size_t>{}(len));
    //  If we want to only scan printable strings:
    ////std::uniform_int_distribution<char> char_dist(' ', '~'); 
    std::uniform_int_distribution<char> char_dist(1, 127); 
    #pragma omp parallel for
    for (size_t i = buffer_off; i < buffer_len - 1; i++) {
        buffer[i] = char_dist(gen);
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

static void BM_strlen_arg(benchmark::State& state) {
    size_t len = state.range(0);
    char* str = new char[len];
    memset(str, 'x', len);
    str[len - 1] = '\0';

    for (auto _ : state) {
#ifdef __BENCH_AOLC__
            benchmark::DoNotOptimize(_strlen(str));
#elif  __BENCH_GLIBC__
            benchmark::DoNotOptimize(strlen(str));
#elif  __BENCH_MUSL__
            __builtin_unreachable();
#else
            __builtin_unreachable();
#endif
            benchmark::ClobberMemory();
    }
    state.SetComplexityN(state.range(0));
    state.SetBytesProcessed(int64_t(state.iterations()) *
                            int64_t(len));

    delete[] str;
}

BENCHMARK(BM_strlen_arg)->RangeMultiplier(2)->Range(1<<10, 1<<18)->Complexity(benchmark::oN);

int main(int argc, char** argv) {
    std::string str_base = "Strlen-Len";
    std::array test_lengths = {8, 16, 32, 64, 256, 1024, 4096};
    for (auto len : test_lengths) {
        benchmark::RegisterBenchmark((str_base + std::to_string(len)).c_str(), BM_Strlen_NBytes,
                                     len, 0);
    }

    std::array misalignment_factors = {1, 7, 8, 15}; // bshrug
    std::array misaligned_test_lengths = {8, 32, 256, 4096, 16384};
    for (auto off : misalignment_factors) {
        str_base = "Strlen-Off" + std::to_string(off) + "-Len";
        for (auto len : misaligned_test_lengths) {
            benchmark::RegisterBenchmark((str_base + std::to_string(len)).c_str(), BM_Strlen_NBytes,
                                         len, off);
        }
    }

//                            1*4096, 2*4096, 3*4096, 4*4096, 5*4096, 6*4096, 7*4096, 8*4096};//, 1, 10, 100, 1000, 10000};

    benchmark::Initialize(&argc, argv);
    benchmark::RunSpecifiedBenchmarks();
}
