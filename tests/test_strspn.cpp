#include "aolc/_test_string.h"
#include <string.h>

#include "gtest/gtest.h"
#include "aolc/compare_buffer_functions.h"

void CompareBufferFuncEvalStrspn(const char* s1, const char* s2, const char* comment) {
    SCOPED_TRACE(comment);
    CompareBufferFunctions<size_t, const char*, const char*>(_strspn, strspn, s1, s2, EqualityMode::kStrictEquality);
}

TEST(strspn, Basic) {
    char str[] = {'x', 'x', 'x', 'X', 'y', 'X', 'X', '\0'};
    char x[]   = "x";
    char xX[]  = "xX";
    char X[]   = "X";
    char y[]   = "y";
    char xyX[] = "xyX";

    CompareBufferFuncEvalStrspn(str, x,   "str, x");
    CompareBufferFuncEvalStrspn(str, xX,  "str, xX");
    CompareBufferFuncEvalStrspn(str, X,   "str, X");
    CompareBufferFuncEvalStrspn(str, y,   "str, y");
    CompareBufferFuncEvalStrspn(str, xyX, "str, xyX");
}

TEST(strspn, Meta) {
    char str[] = {'x', 'x', 'x', 'X', 'y', 'X', 'X', '\0'};
    char x[]   = "x";
    char xX[]  = "xX";
    char X[]   = "X";
    char y[]   = "y";
    char xyX[] = "xyX";

    CompareBufferFuncEvalStrspn(x,   x,   "x, x");
    CompareBufferFuncEvalStrspn(xX,  xX,  "xX, xX");
    CompareBufferFuncEvalStrspn(X,   X,   "X, X");
    CompareBufferFuncEvalStrspn(y,   y,   "y, y");
    CompareBufferFuncEvalStrspn(xyX, xyX, "xyX, xyX");
}
