#include "aolc/_test_string.h"
#include <string.h>

#include "gtest/gtest.h"
#include "aolc/compare_buffer_functions.h"

void CompareBufferFuncEvalStrcspn(const char* s1, const char* s2,
                                  const char* comment) {
    SCOPED_TRACE(comment);
    CompareBufferFunctions<size_t, const char*, const char*>(
        _strcspn, strcspn, s1, s2, EqualityMode::kStrictEquality);
}

TEST(strcspn, Basic) {
    char str[] = {'x', 'x', 'x', 'X', 'y', 'X', 'X', '\0'};
    char x[]   = "x";
    char xX[]  = "xX";
    char X[]   = "X";
    char y[]   = "y";
    char xyX[] = "xyX";

    CompareBufferFuncEvalStrcspn(str, x,   "str, x");
    CompareBufferFuncEvalStrcspn(str, xX,  "str, xX");
    CompareBufferFuncEvalStrcspn(str, X,   "str, X");
    CompareBufferFuncEvalStrcspn(str, y,   "str, y");
    CompareBufferFuncEvalStrcspn(str, xyX, "str, xyX");
}

TEST(strcspn, Meta) {
    char str[] = {'x', 'x', 'x', 'X', 'y', 'X', 'X', '\0'};
    char x[]   = "x";
    char xX[]  = "xX";
    char X[]   = "X";
    char y[]   = "y";
    char xyX[] = "xyX";

    CompareBufferFuncEvalStrcspn(x,   x,   "x, x");
    CompareBufferFuncEvalStrcspn(xX,  xX,  "xX, xX");
    CompareBufferFuncEvalStrcspn(X,   X,   "X, X");
    CompareBufferFuncEvalStrcspn(y,   y,   "y, y");
    CompareBufferFuncEvalStrcspn(xyX, xyX, "xyX, xyX");
}
