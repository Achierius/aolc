#include "aolc/_test_string.h"
#include <string.h>

#include <cstring>
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
