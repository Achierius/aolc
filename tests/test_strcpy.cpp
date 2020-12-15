#include "aolc/_test_string.h"
#include <string.h>

#include "aolc/compare_buffer_functions.h"
#include "gtest/gtest.h"

TEST(strcpy, Basic)
{
        char dest[] = {'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', '\0'};
        char src_nonull[] = {'x', 'x', 'x', 'X', 'X', '\0'};
        char src_yesnull[] = {'y', 'y', 'y', '\0'};
        char goal[] = {'x', 'x', 'y', 'y', 'y', '\0', 'g', 'h', 'i', 'j', '\0'};
        evaluate_strfn(_strcpy, strcpy, dest, src_nonull);
        evaluate_strfn(_strcpy, strcpy, dest, src_yesnull);
}
