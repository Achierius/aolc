#include "string.h"

#include <assert.h>
#include <stdio.h>

void check_strncpy(char* dst, char* src, int n, int offset) {
        //printf("From: \"%s\" To: \"%s\"\n", src, dst);
        strncpy(dst + offset, src + offset, n);
        //printf("Result: \"%s\"\n", dst);
        for (int i = offset; i < offset + n; i++) {
                assert(src[i] == dst[i]);
                if (src[i] == '\0') {
                        return;
                }
        }
}

void test_strncpy() {
        char dest[] = {'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', '\0'};
        char src_nonull[] = {'x', 'x', 'x', 'X', 'X', '\0'};
        char src_yesnull[] = {'y', 'y', 'y', '\0'};
        char goal[] = {'x', 'x', 'x', 'd', 'e', 'y', 'y', 'y', '\0', '\0', '\0'};
        //printf("%s\n", dest);
        //printf("%s\n", src_nonull);
        check_strncpy(dest, src_nonull, 3, 0);
        check_strncpy(dest + 5, src_yesnull, 5, 0);
        for (int i = 0; i < 11; i++) {
                assert(dest[i] == goal[i]);
        }
}

int main () {
        //char dst[] = {'a', 'b', 'c', '\0'};
        //char src[] = {'x', 'x', '\0'};
        //printf("From: \"%s\" To: \"%s\"\n", src, dst);
        //strncpy(dst, src, 4);
        //printf("Result: \"%s\"\n", dst);
        test_strncpy();
        return 0;
}
