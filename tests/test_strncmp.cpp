#include "aolc/_test_string.h"
#include <string.h>
#include <algorithm>

#include "gtest/gtest.h"
#include "aolc/compare_buffer_functions.h"

void CompareStrncmpEval(const char* s1, const char* s2,
                        size_t n, const char* comment) {
    using std::placeholders::_1;
    using std::placeholders::_2;

    auto test_wrapper = std::bind(_strncmp, _1, _2, n);
    auto true_wrapper = std::bind(strncmp,  _1, _2, n);
        
    SCOPED_TRACE(comment);
    CompareBufferFunctions<int, const char*, const char*>(
        test_wrapper, true_wrapper, s1, s2, EqualityMode::kSignEquality);
}

TEST(strncmp, Basic) {
    char str[] = {'x', 'x', 'x', 'X', 'y', 'X', 'X', '\0'};
    char x[]   = "x";
    char xX[]  = "xX";
    char X[]   = "X";
    char y[]   = "y";
    char xyX[] = "xyX";

    CompareStrncmpEval(str, x,
                       std::min(strlen(str), strlen(x)),
                       "str, x");
    CompareStrncmpEval(x,   str,
                       std::min(strlen(str), strlen(x)),
                       "x, str");
    CompareStrncmpEval(str, xX,
                       std::min(strlen(str), strlen(x)),
                       "str, xX");
    CompareStrncmpEval(xX,  str,
                       std::min(strlen(xX), strlen(str)),
                       "xX, str");
    CompareStrncmpEval(str, X,
                       std::min(strlen(X), strlen(str)),
                       "str, X");
    CompareStrncmpEval(X,   str,
                       std::min(strlen(X), strlen(str)),
                       "X, str");
    CompareStrncmpEval(str, y,
                       std::min(strlen(y), strlen(str)),
                       "str, y");
    CompareStrncmpEval(y,   str,
                       std::min(strlen(y), strlen(str)),
                       "y, str");
    CompareStrncmpEval(str, xyX,
                       std::min(strlen(xyX), strlen(str)),
                       "str, xyX");
    CompareStrncmpEval(xyX, str,
                       std::min(strlen(xyX), strlen(str)),
                       "xyX, str");
}

TEST(strncmp, LengthLimit) {
    /* TODO expand */
    char s_xxxA[] = {'x', 'x', 'x', 'A', '\0'};
    char s_xxxB[] = {'x', 'x', 'x', 'B', '\0'};
    char s_xxxC[] = {'x', 'x', 'x', 'C', '\0'};
    CompareStrncmpEval(s_xxxA, s_xxxB, 4, "s_xxA, s_xxB, 4");
    CompareStrncmpEval(s_xxxB, s_xxxC, 4, "s_xxB, s_xxC, 4");
    CompareStrncmpEval(s_xxxC, s_xxxA, 4, "s_xxC, s_xxA, 4");
    CompareStrncmpEval(s_xxxA, s_xxxB, 3, "s_xxA, s_xxB, 3");
    CompareStrncmpEval(s_xxxB, s_xxxC, 3, "s_xxB, s_xxC, 3");
    CompareStrncmpEval(s_xxxC, s_xxxA, 3, "s_xxC, s_xxA, 3");
}

TEST(strncmp, LengthOverflow) {
    char s_xxxA[]  = {'x', 'x', 'x', 'A', '\0'};
    char s_xxxB[]  = {'x', 'x', 'x', 'B', '\0'};
    char s_xxxC[]  = {'x', 'x', 'x', 'C', '\0'};
    CompareStrncmpEval(s_xxxA, s_xxxB, 5,    "s_xxA, s_xxB, 5");
    CompareStrncmpEval(s_xxxB, s_xxxC, 5,    "s_xxB, s_xxC, 5");
    CompareStrncmpEval(s_xxxC, s_xxxA, 5,    "s_xxC, s_xxA, 5");
    CompareStrncmpEval(s_xxxA, s_xxxB, 4096, "s_xxA, s_xxB, 4096");
    CompareStrncmpEval(s_xxxB, s_xxxC, 4096, "s_xxB, s_xxC, 4096");
    CompareStrncmpEval(s_xxxC, s_xxxA, 4096, "s_xxC, s_xxA, 4096");
}

TEST(strncmp, LengthTrim) {
    char s_1[] = {'x', 'x', 'x', 'x', 'C', '\0', 'A', '\0'};
    char s_2[] = {'x', 'x', 'x', 'x', 'C', '\0', 'B', '\0'};

    CompareStrncmpEval(s_1, s_2, 5, "s_1, s_2, 5");
    CompareStrncmpEval(s_1, s_2, 6, "s_1, s_2, 6");
    CompareStrncmpEval(s_1, s_2, 7, "s_1, s_2, 7");
    CompareStrncmpEval(s_1, s_2, 8, "s_1, s_2, 8");

    CompareStrncmpEval(s_2, s_1, 5, "s_2, s_1, 5");
    CompareStrncmpEval(s_2, s_1, 6, "s_2, s_1, 6");
    CompareStrncmpEval(s_2, s_1, 7, "s_2, s_1, 7");
    CompareStrncmpEval(s_2, s_1, 8, "s_2, s_1, 8");
}

TEST(strncmp, Meta) {
    char x[]   = "x";
    char xX[]  = "xX";
    char X[]   = "X";
    char y[]   = "y";
    char xyX[] = "xyX";

    CompareStrncmpEval(x,   x,   1, "x, x");
    CompareStrncmpEval(xX,  xX,  2, "xX, xX");
    CompareStrncmpEval(X,   X,   1, "X, X");
    CompareStrncmpEval(y,   y,   1, "y, y");
    CompareStrncmpEval(xyX, xyX, 3, "xyX, xyX");
}
