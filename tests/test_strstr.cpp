#include "aolc/_test_string.h"
#include <string.h>

#include "aolc/compare_buffer_functions.h"
#include "gtest/gtest.h"

TEST(strstr, Basic) {
    using std::placeholders::_1;
    using std::placeholders::_2;

    char str[] = {'x', 'x', 'x', 'X', 'y', 'X', 'X', '\0'};
    char x[]  = "x";
    char xX[] = "xX";
    char X[]  = "X";
    char y[]  = "y";
    char XX[] = "XX";

    auto CompareBufferFuncEvalStrstr = [&](const char* s1, const char* s2, const char* comment) {
        auto true_fn = std::bind(static_cast<char* (*)(char*, const char*)>(strstr), _1, _2);
        auto test_fn = std::bind(_strstr, _1, _2);
        SCOPED_TRACE(comment);
        CompareBufferFuncEval(test_fn, true_fn, s1, s2);
    };

    CompareBufferFuncEvalStrstr(str, x, "str, x");
    CompareBufferFuncEvalStrstr(str, xX, "str, xX");
    CompareBufferFuncEvalStrstr(str, X, "str, X");
    CompareBufferFuncEvalStrstr(str, y, "str, y");
    CompareBufferFuncEvalStrstr(str, XX, "str, XX");
}
