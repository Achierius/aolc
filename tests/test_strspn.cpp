#include "aolc/_test_string.h"
#include <string.h>

#include "aolc/compare_buffer_functions.h"
#include "gtest/gtest.h"

TEST(strspn, Basic) {
    using std::placeholders::_1;
    using std::placeholders::_2;

    char str[] = {'x', 'x', 'x', 'X', 'y', 'X', 'X', '\0'};
    char x[]   = "x";
    char xX[]  = "xX";
    char X[]   = "X";
    char y[]   = "y";
    char xyX[] = "xyX";

    auto CompareBufferFuncEvalStrspn = [&](const char* acc, const char* rej, const char* comment) {
        auto true_fn = std::bind(strspn, _1, _2);
        auto test_fn = std::bind(_strspn, _1, _2);
        SCOPED_TRACE(comment);
        CompareBufferFuncEval(test_fn, true_fn, s1, s2);
    };

    CompareBufferFuncEvalStrspn(x, x, "x, x");
    CompareBufferFuncEvalStrspn(xX, xX, "xX, xX");
    CompareBufferFuncEvalStrspn(X, X, "X, X");
    CompareBufferFuncEvalStrspn(y, y, "y, y");
    CompareBufferFuncEvalStrspn(xyX, xyX, "xyX, xyX");
}
