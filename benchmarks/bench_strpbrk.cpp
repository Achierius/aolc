#include "string.h"

#ifdef __BENCH_AOLC__
#include <aolc/_test_string.h>
#endif

#include <benchmark/benchmark.h>

#include <string>
#include <random>
#include <array>

static void BM_strpbrk(benchmark::State& state, size_t len, size_t n_tokens) {
    char* str_buffer = new char[len + 1];
    char* tok_buffer = new char[n_tokens + 1];

    std::random_device rd;
    std::uniform_int_distribution<char> char_dist(1, 127); 

    for (size_t i = 0; i < len; i++) {
        str_buffer[i] = char_dist(rd);
    }
    str_buffer[len] = '\0';

    for (size_t i = 0; i < n_tokens; i++) {
        tok_buffer[i] = char_dist(rd);
    }
    tok_buffer[n_tokens] = '\0';

    for (auto _ : state) {
            // Only the contents of this loop get benchmarked
#ifdef __BENCH_AOLC__
            benchmark::DoNotOptimize(_strpbrk(str_buffer, tok_buffer));
#elif  __BENCH_GLIBC__
            benchmark::DoNotOptimize(strpbrk(str_buffer, tok_buffer));
#elif  __BENCH_MUSL__
            __builtin_unreachable();
#else
            __builtin_unreachable();
#endif
            benchmark::ClobberMemory();
    }

    delete[](str_buffer);
    delete[](tok_buffer);
}

int main(int argc, char** argv) {
    auto NameTest = [](int len, int n_tokens){
        return "strpbrk_StrLen" + std::to_string(len) + "-TokLen" + std::to_string(n_tokens);
    };
    std::array<int, 9> test_lengths = {4, 16, 64, 256, 4096, 16384, 1, 100, 10000};
    char test_char = '~';
    for (auto tok_len : {1, 2, 8, 32, 128}) {
        for (auto len : test_lengths) {
            benchmark::RegisterBenchmark(NameTest(len, tok_len).c_str(), BM_strpbrk, len, tok_len);
        }
    }
    benchmark::Initialize(&argc, argv);
    benchmark::RunSpecifiedBenchmarks();
}
