#include "aolc/_test_string.h"
#include <string.h>

#include "gtest/gtest.h"
#include "aolc/compare_buffer_functions.h"

void CompareStrcollEval(const char* s1, const char* s2,
                        const char* comment) {
    SCOPED_TRACE(comment);
    CompareBufferFunctions<int, const char*, const char*>(
        _strcoll, strcoll, s1, s2, EqualityMode::kSignEquality);
}

/* TODO: We _definitely_ need a setup to actually test the handling of
 *       different collating orders. */

TEST(strcoll, Basic) {
    char str[] = {'x', 'x', 'x', 'X', 'y', 'X', 'X', '\0'};
    char x[]   = "x";
    char xX[]  = "xX";
    char X[]   = "X";
    char y[]   = "y";
    char xyX[] = "xyX";

    CompareStrcollEval(str, x,   "str, x");
    CompareStrcollEval(x,   str, "x, str");
    CompareStrcollEval(str, xX,  "str, xX");
    CompareStrcollEval(xX,  str, "xX, str");
    CompareStrcollEval(str, X,   "str, X");
    CompareStrcollEval(X,   str, "X, str");
    CompareStrcollEval(str, y,   "str, y");
    CompareStrcollEval(y,   str, "y, str");
    CompareStrcollEval(str, xyX, "str, xyX");
    CompareStrcollEval(xyX, str, "xyX, str");
}

TEST(strcoll, Meta) {
    char x[]   = "x";
    char xX[]  = "xX";
    char X[]   = "X";
    char y[]   = "y";
    char xyX[] = "xyX";

    CompareStrcollEval(x,   x,   "x, x");
    CompareStrcollEval(xX,  xX,  "xX, xX");
    CompareStrcollEval(X,   X,   "X, X");
    CompareStrcollEval(y,   y,   "y, y");
    CompareStrcollEval(xyX, xyX, "xyX, xyX");
}
