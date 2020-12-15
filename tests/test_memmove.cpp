#include <string.h>
#include "aolc/_test_string.h"
#include "aolc/compare_buffer_functions.h"
#include "gtest/gtest.h"

#include <assert.h>
#include <stdio.h>

TEST(memmove, Basic) {
        char s1[] = {'a', 'a', 'a', 'a', 'a', 's', 's', 's', 's', 's', 's', 'd', 'd', 'd', 'd', 'd', 'd', 'd', 'd', 'd', 'd', 'd'};
        char s2[] = {'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x'};
        char s3[] = ":)";

       evaluate_memfn(_memmove, memmove, s1, s2, 20);
       evaluate_memfn(_memmove, memmove, s2, s1, 22);
       evaluate_memfn(_memmove, memmove, s3, s2, 2);
       evaluate_memfn(_memmove, memmove, s3, s1, 2);
       evaluate_memfn(_memmove, memmove, s1, s2, 4);
}

TEST(memmove, Overlapping) {
        char s1[] = {'a', 'a', 'a', 'a', 'a', 's', 's', 's', 's', 's', 's', 'd', 'd', 'd', 'd', 'd', 'd', 'd', 'd', 'd', 'd', 'd'};
        char s2[] = {'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x'};

       evaluate_memfn(_memmove, memmove, s1 + 0, s1 + 9, 20);
       evaluate_memfn(_memmove, memmove, s2 + 8, s2 + 0, 10);
       evaluate_memfn(_memmove, memmove, s1 + 1, s2 + 1, 25);
}
