#include "aolc/_test_string.h"
#include <string.h>

#include "aolc/compare_buffer_functions.h"
#include "gtest/gtest.h"

TEST(StrlenCorrectness, Basic)
{
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
