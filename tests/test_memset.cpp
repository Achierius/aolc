#include "internal/_test_string.h"
#include <string.h>

#include "internal/compare_buffer_functions.h"
#include "gtest/gtest.h"

TEST(memset, Basic)
{
        char s_test[] = {"aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa\0"};
        char s_true[] = {"aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa\0"};
        
        memset(s_true + 3, 'A', 3);
        memset(s_true + 10, 'x', 7);
        memset(s_true + 20, '\0', 1);

        _memset(s_test + 3, 'A', 3);
        _memset(s_test + 10, 'x', 7);
        _memset(s_test + 20, '\0', 1);
        EXPECT_STREQ(s_true, s_test);
}
