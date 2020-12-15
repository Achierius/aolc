#include <string.h>
#include "aolc/_test_string.h"
#include "aolc/compare_buffer_functions.h"
#include "gtest/gtest.h"

#include <assert.h>
#include <stdio.h>

TEST(memcpy, Basic) {
        char s_unused_A[] = {'a', 's', 'd', '\0'};
        char s_unused_B[] = {'x', 'x', 'x', '\0'};
        char s1[] = {'a', 'a', 'a', 'a', 'a', 's', 's', 's', 's', 's', 's', 'd', 'd', 'd', 'd', 'd', 'd', 'd', 'd', 'd', 'd', 'd'};
        char s2[] = {'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x'};
        char s3[] = ":)";

       evaluate_memfn(_memcpy, memcpy, s1, s2, 20);
       evaluate_memfn(_memcpy, memcpy, s2, s1, 22);
       evaluate_memfn(_memcpy, memcpy, s3, s2, 2);
       evaluate_memfn(_memcpy, memcpy, s3, s1, 2);
       evaluate_memfn(_memcpy, memcpy, s1, s2, 4);
}
