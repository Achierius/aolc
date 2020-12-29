#include "aolc/_test_string.h"
#include <string.h>

#include "gtest/gtest.h"
#include "aolc/compare_buffer_functions.h"

void CompareStrcmpEval(const char* s1, const char* s2,
                       const char* comment) {
    SCOPED_TRACE(comment);
    CompareBufferFunctions<int, const char*, const char*>(
        _strcmp, strcmp, s1, s2, EqualityMode::kSignEquality);
}

TEST(strcmp, Basic) {
    char str[] = {'x', 'x', 'x', 'X', 'y', 'X', 'X', '\0'};
    char x[]   = "x";
    char xX[]  = "xX";
    char X[]   = "X";
    char y[]   = "y";
    char xyX[] = "xyX";

    CompareStrcmpEval(str, x,   "str, x");
    CompareStrcmpEval(x,   str, "x, str");
    CompareStrcmpEval(str, xX,  "str, xX");
    CompareStrcmpEval(xX,  str, "xX, str");
    CompareStrcmpEval(str, X,   "str, X");
    CompareStrcmpEval(X,   str, "X, str");
    CompareStrcmpEval(str, y,   "str, y");
    CompareStrcmpEval(y,   str, "y, str");
    CompareStrcmpEval(str, xyX, "str, xyX");
    CompareStrcmpEval(xyX, str, "xyX, str");
}

TEST(strcmp, Meta) {
    char x[]   = "x";
    char xX[]  = "xX";
    char X[]   = "X";
    char y[]   = "y";
    char xyX[] = "xyX";

    CompareStrcmpEval(x,   x,   "x, x");
    CompareStrcmpEval(xX,  xX,  "xX, xX");
    CompareStrcmpEval(X,   X,   "X, X");
    CompareStrcmpEval(y,   y,   "y, y");
    CompareStrcmpEval(xyX, xyX, "xyX, xyX");
}
