#include "aolc/_test_string.h"
#include <string.h>

#include "aolc/compare_buffer_functions.h"
#include "gtest/gtest.h"

TEST(strspn, Basic) {
    char str[] = {'x', 'x', 'x', 'X', 'y', 'X', 'X', '\0'};
    char x[]   = "x";
    char xX[]  = "xX";
    char X[]   = "X";
    char y[]   = "y";
    char xyX[] = "xyX";

    EXPECT_EQ(strspn(str, x), _strspn(str, x));
    EXPECT_EQ(strspn(str, xX), _strspn(str, xX));
    EXPECT_EQ(strspn(str, X), _strspn(str, X));
    EXPECT_EQ(strspn(str, y), _strspn(str, y));
    EXPECT_EQ(strspn(str, xyX), _strspn(str, xyX));
}
