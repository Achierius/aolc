#include "aolc/_test_string.h"
#include <string.h>

#include "gtest/gtest.h"
#include "aolc/compare_buffer_functions.h"

void CompareMemcmpEval(const char* s1, const char* s2, size_t n,
                       const char* comment) {
    using std::placeholders::_1;
    using std::placeholders::_2;

    auto test_wrapper = std::bind(_memcmp, _1, _2, n);
    auto true_wrapper = std::bind(memcmp,  _1, _2, n);

    SCOPED_TRACE(comment);
    CompareBufferFunctions<int, const void*, const void*>(
        test_wrapper, true_wrapper, s1, s2, n, n, EqualityMode::kSignEquality);
}

TEST(memcmp, Basic) {
    char str[] = {'x', 'x', 'x', 'X', 'y', 'X', 'X', '\0'};
    char x[]   = "x";
    char xX[]  = "xX";
    char X[]   = "X";
    char y[]   = "y";
    char xyX[] = "xyX";

    CompareMemcmpEval(str, x,   1, "str, x");
    CompareMemcmpEval(x,   str, 1, "x, str");
    CompareMemcmpEval(str, xX,  2, "str, xX");
    CompareMemcmpEval(xX,  str, 2, "xX, str");
    CompareMemcmpEval(str, X,   1, "str, X");
    CompareMemcmpEval(X,   str, 1, "X, str");
    CompareMemcmpEval(str, y,   1, "str, y");
    CompareMemcmpEval(y,   str, 1, "y, str");
    CompareMemcmpEval(str, xyX, 3, "str, xyX");
    CompareMemcmpEval(xyX, str, 3, "xyX, str");
}

TEST(memcmp, Meta) {
    char x[]   = "x";
    char xX[]  = "xX";
    char X[]   = "X";
    char y[]   = "y";
    char xyX[] = "xyX";

    CompareMemcmpEval(x,   x,   1, "x, x");
    CompareMemcmpEval(xX,  xX,  2, "xX, xX");
    CompareMemcmpEval(X,   X,   1, "X, X");
    CompareMemcmpEval(y,   y,   1, "y, y");
    CompareMemcmpEval(xyX, xyX, 3, "xyX, xyX");
}
