#include "aolc/_test_string.h"
#include <string.h>

#include "aolc/compare_buffer_functions.h"
#include "gtest/gtest.h"

TEST(strpbrk, Basic) {
    using std::placeholders::_1;
    using std::placeholders::_2;

    char str[] = {'x', 'x', 'x', 'X', 'y', 'X', 'X', '\0'};
    char x[]  = "x";
    char xX[] = "xX";
    char X[]  = "X";
    char y[]  = "y";
    char _y[] = "asdasdasdasdasdasdaasdgqweqweasdqweqqe_y";

    auto CompareBufferFuncEvalStrpbrk = [&](const char* s1, const char* s2, const char* comment) {
        auto true_fn = std::bind(strpbrk, _1, _2);
        auto test_fn = std::bind(_strpbrk, _1, _2);
        SCOPED_TRACE(comment);
        CompareBufferFuncEval(test_fn, true_fn, s1, s2);
    };

    CompareBufferFuncEvalStrpbrk(str, x, "str, x");
    CompareBufferFuncEvalStrpbrk(str, xX, "str, xX");
    CompareBufferFuncEvalStrpbrk(str, X, "str, X");
    CompareBufferFuncEvalStrpbrk(str, y, "str, y");
    CompareBufferFuncEvalStrpbrk(str, str, "str, str");
    CompareBufferFuncEvalStrpbrk(str, _y, "str, _y");
}
