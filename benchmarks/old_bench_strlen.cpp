#include <benchmark/benchmark.h>
#include <string.h>
#include <aolc/_test_string.h>

/* Word-Aligned */
char len_128[] = "Abcdefg.Abcdefg.Abcdefg.Abcdefg.Abcdefg.Abcdefg.Abcdefg.Abcdefg.Abcdefg.Abcdefg.Abcdefg.Abcdefg.Abcdefg.Abcdefg.Abcdefg.Abcdefg.";
char len_064[] = "Abcdefg.Abcdefg.Abcdefg.Abcdefg.Abcdefg.Abcdefg.Abcdefg.Abcdefg.";
char len_032[] = "Abcdefg.Abcdefg.Abcdefg.Abcdefg.";
char len_016[] = "Abcdefg.Abcdefg.";
char len_008[] = "Abcdefg.";
char len_004[] = "Nice";

/* This is definitely the best way to do this :) */
char buff_1024[] = "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!";
char* len_1024 = buff_1024;
char* len_0512 = buff_1024 + 512;
char* len_0256 = buff_1024 + 512 + 256;

static void BM_Strlen_aolc_8B_Misaligned(benchmark::State& state) {
    char* test_str = len_008 + 1;
    volatile int t = 0;
    for (auto _ : state) {
        t = _strlen(test_str);
    }
}

static void BM_Strlen_glibc_8B_Misaligned(benchmark::State& state) {
    char* test_str = len_008 + 1;
    volatile int t = 0;
    for (auto _ : state) {
        t = strlen(test_str);
    }
}

static void BM_Strlen_aolc_8B(benchmark::State& state) {
    char* test_str = len_008;
    volatile int t = 0;
    for (auto _ : state) {
        t = _strlen(test_str);
    }
}

static void BM_Strlen_glibc_8B(benchmark::State& state) {
    char* test_str = len_008;
    volatile int t = 0;
    for (auto _ : state) {
        t = strlen(test_str);
    }
}

static void BM_Strlen_aolc_32B(benchmark::State& state) {
    char* test_str = len_032;
    volatile int t = 0;
    for (auto _ : state) {
        t = _strlen(test_str);
    }
}

static void BM_Strlen_glibc_32B(benchmark::State& state) {
    char* test_str = len_032;
    volatile int t = 0;
    for (auto _ : state) {
        t = strlen(test_str);
    }
}

static void BM_Strlen_aolc_64B(benchmark::State& state) {
    char* test_str = len_064;
    volatile int t = 0;
    for (auto _ : state) {
        t = _strlen(test_str);
    }
}

static void BM_Strlen_glibc_64B(benchmark::State& state) {
    char* test_str = len_064;
    volatile int t = 0;
    for (auto _ : state) {
        t = strlen(test_str);
    }
}

static void BM_Strlen_aolc_128B(benchmark::State& state) {
    char* test_str = len_128;
    volatile int t = 0;
    for (auto _ : state) {
        t = _strlen(test_str);
    }
}

static void BM_Strlen_glibc_128B(benchmark::State& state) {
    char* test_str = len_128;
    volatile int t = 0;
    for (auto _ : state) {
        t = strlen(test_str);
    }
}

static void BM_Strlen_aolc_256B(benchmark::State& state) {
    char* test_str = len_0256;
    volatile int t = 0;
    for (auto _ : state) {
        t = _strlen(test_str);
    }
}

static void BM_Strlen_glibc_256B(benchmark::State& state) {
    char* test_str = len_0256;
    volatile int t = 0;
    for (auto _ : state) {
        t = strlen(test_str);
    }
}

static void BM_Strlen_aolc_512B(benchmark::State& state) {
    char* test_str = len_0512;
    volatile int t = 0;
    for (auto _ : state) {
        t = _strlen(test_str);
    }
}

static void BM_Strlen_glibc_512B(benchmark::State& state) {
    char* test_str = len_0512;
    volatile int t = 0;
    for (auto _ : state) {
        t = strlen(test_str);
    }
}

static void BM_Strlen_aolc_1024B(benchmark::State& state) {
    char* test_str = len_1024;
    volatile int t = 0;
    for (auto _ : state) {
        t = _strlen(test_str);
    }
}

static void BM_Strlen_glibc_1024B(benchmark::State& state) {
    char* test_str = len_1024;
    volatile int t = 0;
    for (auto _ : state) {
        t = strlen(test_str);
    }
}

static void BM_Strlen_aolc_1024B_Misaligned(benchmark::State& state) {
    char* test_str = len_1024 + 1;
    volatile int t = 0;
    for (auto _ : state) {
        t = _strlen(test_str);
    }
}

static void BM_Strlen_glibc_1024B_Misaligned(benchmark::State& state) {
    char* test_str = len_1024 + 1;
    volatile int t = 0;
    for (auto _ : state) {
        t = strlen(test_str);
    }
}

static void BM_Strlen_aolc_32768B(benchmark::State&  state) {
    char* huge_buffer = (char*) calloc(1, 32768);
    memset(huge_buffer, '~', 32000);
    volatile int t = 0;
    for (auto _ : state) {
        t = _strlen(huge_buffer);
    }
}

static void BM_Strlen_glibc_32768B(benchmark::State&  state) {
    char* huge_buffer = (char*) calloc(1, 32768);
    memset(huge_buffer, '~', 32000);
    volatile int t = 0;
    for (auto _ : state) {
        t = strlen(huge_buffer);
    }
}

BENCHMARK(BM_Strlen_aolc_8B_Misaligned);
BENCHMARK(BM_Strlen_aolc_8B);
BENCHMARK(BM_Strlen_aolc_32B);
BENCHMARK(BM_Strlen_aolc_64B);
BENCHMARK(BM_Strlen_aolc_128B);
BENCHMARK(BM_Strlen_aolc_256B);
BENCHMARK(BM_Strlen_aolc_512B);
BENCHMARK(BM_Strlen_aolc_1024B);
BENCHMARK(BM_Strlen_aolc_1024B_Misaligned);
BENCHMARK(BM_Strlen_aolc_32768B);
BENCHMARK(BM_Strlen_glibc_8B_Misaligned);
BENCHMARK(BM_Strlen_glibc_8B);
BENCHMARK(BM_Strlen_glibc_32B);
BENCHMARK(BM_Strlen_glibc_64B);
BENCHMARK(BM_Strlen_glibc_128B);
BENCHMARK(BM_Strlen_glibc_256B);
BENCHMARK(BM_Strlen_glibc_512B);
BENCHMARK(BM_Strlen_glibc_1024B);
BENCHMARK(BM_Strlen_glibc_1024B_Misaligned);
BENCHMARK(BM_Strlen_glibc_32768B);

BENCHMARK_MAIN();
