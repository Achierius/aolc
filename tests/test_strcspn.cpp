#include "internal/_test_string.h"
#include <string.h>

#include "internal/compare_buffer_functions.h"
#include "gtest/gtest.h"

TEST(strcspn, Basic) {
    char str[] = {'x', 'x', 'x', 'X', 'y', 'X', 'X', '\0'};
    char x[]   = "x";
    char xX[]  = "xX";
    char X[]   = "X";
    char y[]   = "y";
    char xyX[] = "xyX";

    EXPECT_EQ(strcspn(str, x), _strcspn(str, x));
    EXPECT_EQ(strcspn(str, xX), _strcspn(str, xX));
    EXPECT_EQ(strcspn(str, X), _strcspn(str, X));
    EXPECT_EQ(strcspn(str, y), _strcspn(str, y));
    EXPECT_EQ(strcspn(str, xyX), _strcspn(str, xyX));
}
