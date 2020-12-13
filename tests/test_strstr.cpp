#include "internal/_test_string.h"
#include <string.h>

#include "internal/compare_buffer_functions.h"
#include "gtest/gtest.h"


char* _strstr_wrap(char* s1, const char* s2) {
    return _strstr(s1, s2);
}

char* strstr_wrap(char* s1, const char* s2) {
    return strstr(s1, s2);
}

TEST(strstr, Basic) {
    char str[] = {'x', 'x', 'x', 'X', 'y', 'X', 'X', '\0'};
    char x[]  = "x";
    char xX[] = "xX";
    char X[]  = "X";
    char y[]  = "y";
    char XX[] = "XX";

    evaluate_strfn(_strstr_wrap, strstr_wrap, str, x);
    evaluate_strfn(_strstr_wrap, strstr_wrap, str, xX);
    evaluate_strfn(_strstr_wrap, strstr_wrap, str, X);
    evaluate_strfn(_strstr_wrap, strstr_wrap, str, y);
    evaluate_strfn(_strstr_wrap, strstr_wrap, str, XX);
}
