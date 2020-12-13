#include "internal/_test_string.h"
#include <string.h>

#include "internal/compare_buffer_functions.h"
#include "gtest/gtest.h"

TEST(strncpy, Basic)
{
        char dest[] = {'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', '\0'};
        char src_nonull[] = {'x', 'x', 'x', 'X', 'X', '\0'};
        char src_yesnull[] = {'y', 'y', 'y', '\0'};
        char goal[] = {'x', 'x', 'x', 'd', 'e', 'y', 'y', 'y', '\0', '\0', '\0'};

        evaluate_strnfn(_strncpy, strncpy, dest, src_nonull, 3);
        evaluate_strnfn(_strncpy, strncpy, dest + 5, src_yesnull, 3);
        evaluate_strnfn(_strncpy, strncpy, goal, src_yesnull, 8);
}
