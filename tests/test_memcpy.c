#include "string.h"

#include <assert.h>
#include <stdio.h>

void check_memcpy(char* src, char* dst, size_t n) {
        memcpy(dst, src, n);
        for (int i = 0; i < n; i++) {
                assert(dst[i] == src[i]);
        }
}

void test_memcpy() {
        char s_unused_A[] = {'a', 's', 'd', '\0'};
        char s_unused_B[] = {'x', 'x', 'x', '\0'};
        char s1[] = {'a', 'a', 'a', 'a', 'a', 's', 's', 's', 's', 's', 's', 'd', 'd', 'd', 'd', 'd', 'd', 'd', 'd', 'd', 'd', 'd'};
        char s2[] = {'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x'};
        char s3[] = ":)";
        check_memcpy(s1, s2, 20);
        check_memcpy(s2, s1, 22);
        check_memcpy(s3, s2, 2);
        check_memcpy(s3, s1, 2);
        check_memcpy(s1, s2, 4);
}

int main()
{
        test_memcpy();
        return 0;
}
