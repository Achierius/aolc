#include <string.h>

#include <assert.h>
#include <stdio.h>

void check_strcpy(char* dst, char* src) {
        strcpy(dst, src);
        for (int i = 0; ; i++) {
                assert(src[i] == dst[i]);
                if (src[i] == '\0') {
                        return;
                }
        }
}

void test_strcpy() {
        char dest[] = {'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', '\0'};
        char src_nonull[] = {'x', 'x', 'x', 'X', 'X', '\0'};
        char src_yesnull[] = {'y', 'y', 'y', '\0'};
        char goal[] = {'x', 'x', 'y', 'y', 'y', '\0', 'g', 'h', 'i', 'j', '\0'};
        check_strcpy(dest, src_nonull);
        check_strcpy(dest + 2, src_yesnull);
        for (int i = 0; i < 11; i++) {
                assert(dest[i] == goal[i]);
        }
}

int main () {
        test_strcpy();
        return 0;
}
