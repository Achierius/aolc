#include "string.h"
#ifdef __BENCH_AOLC__
#include <aolc/_test_string.h>
#endif

#include <benchmark/benchmark.h>

#include <string>
#include <random>
#include <array>

// All tokens WILL be in the string SOMEWHERE (with this impl)
static void BM_strrchr(benchmark::State& state, size_t len) {
    char* str_buffer = new char[len + 1];
    char  tok = '\0';

    std::mt19937 char_gen(std::hash<size_t>{}(len + 1));
    std::mt19937 index_gen(std::hash<size_t>{}(len));

    std::uniform_int_distribution<char> char_dist(1, 127); 
    std::uniform_int_distribution<char> index_dist(0, len); 

    for (size_t i = 0; i < len; i++) {
        str_buffer[i] = char_dist(char_gen);
    }

    str_buffer[len - 1] = 127;
    str_buffer[len] = '\0';

    tok = 127;

    for (auto _ : state) {
            // Only the contents of this loop get benchmarked
#ifdef __BENCH_AOLC__
            benchmark::DoNotOptimize(_strrchr(str_buffer, tok));
#elif  __BENCH_GLIBC__
            benchmark::DoNotOptimize(strrchr(str_buffer, tok));
#elif  __BENCH_MUSL__
            __builtin_unreachable();
#else
            __builtin_unreachable();
#endif
            benchmark::ClobberMemory();
    }

    delete[](str_buffer);
}

int main(int argc, char** argv) {
    auto NameTest = [](int len){
        return "strrchr_Len" + std::to_string(len);
    };
    std::array<int, 9> test_lengths = {4, 16, 64, 256, 4096, 16384, 1, 100, 10000};
    for (auto len : test_lengths) {
        benchmark::RegisterBenchmark(NameTest(len).c_str(), BM_strrchr, len);
    }
    benchmark::Initialize(&argc, argv);
    benchmark::RunSpecifiedBenchmarks();
}
