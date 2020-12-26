#include "aolc/_test_string.h"
#include <string.h>

#include "aolc/compare_buffer_functions.h"
#include "gtest/gtest.h"

void CompareStrpbrkEval(const char* s1,
                        const char* s2,
                        const char* comment) {
    using std::placeholders::_1;
    using std::placeholders::_2;

    SCOPED_TRACE(comment);
    CompareBufferFunctions<char, char, const char>(
        _strpbrk, static_cast<char* (*)(char*, const char*)>(strpbrk), s1, s2);
}

TEST(strpbrk, Basic) {
    char str[] = {'x', 'x', 'x', 'X', 'y', 'X', 'X', '\0'};
    char x[]  = "x";
    char xX[] = "xX";
    char X[]  = "X";
    char y[]  = "y";
    char _y[] = "asdasdasdasdasdasdaasdgqweqweasdqweqqe_y";

    CompareStrpbrkEval(str, x, "str, x");
    CompareStrpbrkEval(str, xX, "str, xX");
    CompareStrpbrkEval(str, X, "str, X");
    CompareStrpbrkEval(str, y, "str, y");
    CompareStrpbrkEval(str, str, "str, str");
    CompareStrpbrkEval(str, _y, "str, _y");
}
