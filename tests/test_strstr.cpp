#include "aolc/_test_string.h"
#include <string.h>

#include "aolc/compare_buffer_functions.h"
#include "gtest/gtest.h"

void CompareStrstrEval(const char* s1, const char* s2, const char* comment) {
    using std::placeholders::_1;
    using std::placeholders::_2;

    auto true_fn = std::bind(static_cast<char* (*)(char*, const char*)>(strstr), _1, _2);
    auto test_fn = std::bind(_strstr, _1, _2);
    SCOPED_TRACE(comment);
    CompareBufferFunctions<char, char, const char>(test_fn, true_fn, s1, s2);
}

TEST(strstr, Basic) {
    char str[] = {'x', 'x', 'x', 'X', 'y', 'X', 'X', '\0'};
    char x[]  = "x";
    char xX[] = "xX";
    char X[]  = "X";
    char y[]  = "y";
    char XX[] = "XX";

    CompareStrstrEval(str, x, "str, x");
    CompareStrstrEval(str, xX, "str, xX");
    CompareStrstrEval(str, X, "str, X");
    CompareStrstrEval(str, y, "str, y");
    CompareStrstrEval(str, XX, "str, XX");
}
