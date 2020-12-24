#include "aolc/_test_string.h"
#include <string.h>

#include "aolc/compare_buffer_functions.h"
#include "gtest/gtest.h"

TEST(strcspn, Basic) {
    using std::placeholders::_1;
    using std::placeholders::_2;

    char str[] = {'x', 'x', 'x', 'X', 'y', 'X', 'X', '\0'};
    char x[]   = "x";
    char xX[]  = "xX";
    char X[]   = "X";
    char y[]   = "y";
    char xyX[] = "xyX";

    auto CompareBufferFuncEvalStrcspn = [&](const char* acc, const char* rej, const char* comment) {
        auto true_fn = std::bind(strcspn, _1, _2);
        auto test_fn = std::bind(_strcspn, _1, _2);
        SCOPED_TRACE(comment);
        CompareBufferFuncEval(test_fn, true_fn, s1, s2);
    };

    CompareBufferFuncEvalStrcspn(x, x, "x, x");
    CompareBufferFuncEvalStrcspn(xX, xX, "xX, xX");
    CompareBufferFuncEvalStrcspn(X, X, "X, X");
    CompareBufferFuncEvalStrcspn(y, y, "y, y");
    CompareBufferFuncEvalStrcspn(xyX, xyX, "xyX, xyX");
}
