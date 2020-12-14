#include "internal/_test_string.h"
#include <string.h>

#include "internal/compare_buffer_functions.h"
#include "gtest/gtest.h"

TEST(strpbrk, Basic) {
    char str[] = {'x', 'x', 'x', 'X', 'y', 'X', 'X', '\0'};
    char x[]  = "x";
    char xX[] = "xX";
    char X[]  = "X";
    char y[]  = "y";
    char _y[] = "asdasdasdasdasdasdaasdgqweqweasdqweqqe_y";

    auto _strpbrkW = [](char* s1, const char* s2){ return _strpbrk(s1 ,s2); };
    auto  strpbrkW = [](char* s1, const char* s2){ return  strpbrk(s1 ,s2); };
    evaluate_strfn(_strpbrkW, strpbrkW, str, x);
    evaluate_strfn(_strpbrkW, strpbrkW, str, xX);
    evaluate_strfn(_strpbrkW, strpbrkW, str, X);
    evaluate_strfn(_strpbrkW, strpbrkW, str, y);
    evaluate_strfn(_strpbrkW, strpbrkW, str, str);
    evaluate_strfn(_strpbrkW, strpbrkW, str, _y);
}
