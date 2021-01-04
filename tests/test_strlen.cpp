#include "aolc/_test_string.h"
#include <string.h>

#include <string>
#include <random>
#include <array>
#include <functional>
#include <algorithm>
#include "aolc/compare_buffer_functions.h"
#include "gtest/gtest.h"

void CompareStrlenEval(const char* s1,
                       const char* comment) {
    SCOPED_TRACE(comment);
    CompareBufferFunctions<size_t, const char*>(
        _strlen, strlen, s1, strlen(s1), EqualityMode::kStrictEquality);
}

TEST(strlen, AlignedSimple) {
    const char* __attribute__ ((aligned (16))) str_1 = "str";
    const char* __attribute__ ((aligned (16))) str_2 = "string string";

    CompareStrlenEval(str_1, "str_1");
    CompareStrlenEval(str_2, "str_2");
}

TEST(strlen, Aligned) {
    const char* __attribute__ ((aligned (16))) str_1 = "test string one";
    const char* __attribute__ ((aligned (16))) str_2 = "test string two, thus time non-insignificantly longer";
    const char* __attribute__ ((aligned (16))) str_3 = "test string three; really, I could keep going all day, "
                                                       "but maybe this is the longest I'll go for this one --"
                                                       "definitely crosses a few 128b boundaries at this point, ja?";
    const char* __attribute__ ((aligned (16))) str_4 = "test string four: \f\n white \t \v \f\f\n \r space! \n\t\b";
    const char* __attribute__ ((aligned (16))) str_5 = "\x001 test \x002\x002 string five: \x004 special \x020 characters :)";

    CompareStrlenEval(str_1, "str_1");
    CompareStrlenEval(str_2, "str_2");
    CompareStrlenEval(str_3, "str_3");
    CompareStrlenEval(str_4, "str_4");
    CompareStrlenEval(str_5, "str_5");
}

TEST(strlen, Basic) {
    char s1[] = "nice string";
    char s2[] = {123, 11, 13, 100, 110, 101, 2, 123, 55, 106, 72, 66, 66, 66, 1, 0};
    char s3[] = "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa\0";
    char s4[] = "aaaaaaaa aaacaaaaaaaabaaaaaaaaa aaaabaaaaaaaaaaaaaaaaaaaaabaaaaaaaaaaaaaa\0";
    char s5[] = "aaaaaaaa aaacaaaaaa\0abaaaaaaaaa aaaabaaa\0aaaaaaaaaaaaaaaaaaba\0";
    char s6[] = {'\0', '\0', '\0'};
    /* TODO MORE? */
    EXPECT_EQ(_strlen(s1), strlen(s1));
    EXPECT_EQ(_strlen(s2), strlen(s2));
    EXPECT_EQ(_strlen(s3), strlen(s3));
    EXPECT_EQ(_strlen(s4), strlen(s4));
    EXPECT_EQ(_strlen(s5), strlen(s5));
    EXPECT_EQ(_strlen(s5 + 20), strlen(s5 + 20));
    EXPECT_EQ(_strlen(s6), strlen(s6));
}

TEST(strlen, Benchmark) {
    std::array misalignment_factors = {0, 1, 7, 8, 15};
    std::array misaligned_test_lengths = {8, 32, 256, 2048}; // 4096, 16384}; // Too longer for our test buffers :((

    size_t max_len = *std::max_element(misaligned_test_lengths.begin(), misaligned_test_lengths.end());
    size_t buf_len = 2*max_len + 1;
    char* buffer = new char[buf_len];
    std::uniform_int_distribution<char> char_dist(1, 127); 
    for (auto off : misalignment_factors) {
        SCOPED_TRACE("offset: " + std::to_string(off));
        for (auto len : misaligned_test_lengths) {
            SCOPED_TRACE("length: " + std::to_string(len));
            std::mt19937 gen(std::hash<size_t>{}(len));
            memset(buffer, '\0', buf_len);
            for (size_t i = off; i < off + len - 1; i++) {
                buffer[i] = char_dist(gen);
            }

            CompareStrlenEval(buffer + off, ("strlen len_" + std::to_string(len) +
                                                   " off_" + std::to_string(off)).c_str());

        }
    }
    delete[] buffer;
}

TEST(strlen, Alignments) {
    const size_t kBufLen = 128;
    const size_t kBaseOffset = 0;
    const size_t kMaxOffset = 64;

    char __attribute__ ((aligned (64))) buffer[kBufLen] = {'\0'}; 

    std::uniform_int_distribution<char> char_dist(1, 127); 
    std::mt19937 gen(std::hash<size_t>{}(kBufLen));
    for (size_t i = 0; i < kBufLen - 1; i++) {
        buffer[i] = char_dist(gen);
    }

    for (size_t off = kBaseOffset; off < kMaxOffset; off++) {
        CompareStrlenEval(buffer + off, ("strlen aligned " + std::to_string(off) + " (mod 64)").c_str()); 
    }
}
