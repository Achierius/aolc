#include "aolc/_test_string.h"
#include <string.h>

#include "aolc/compare_buffer_functions.h"
#include "gtest/gtest.h"

TEST(strstr, Basic) {
    char str[] = {'x', 'x', 'x', 'X', 'y', 'X', 'X', '\0'};
    char x[]  = "x";
    char xX[] = "xX";
    char X[]  = "X";
    char y[]  = "y";
    char XX[] = "XX";

    auto _strstrW = [](char* s1, const char* s2){ return _strstr(s1 ,s2); };
    auto  strstrW = [](char* s1, const char* s2){ return  strstr(s1 ,s2); };
    evaluate_strfn(_strstrW, strstrW, str, x);
    evaluate_strfn(_strstrW, strstrW, str, xX);
    evaluate_strfn(_strstrW, strstrW, str, X);
    evaluate_strfn(_strstrW, strstrW, str, y);
    evaluate_strfn(_strstrW, strstrW, str, XX);
}
