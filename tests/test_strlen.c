#include "string.h"
#include <stdio.h>
#include <assert.h>


void test_strlen() {
        extern size_t __strlen();

        char s1[] = "nice string";
        char s2[] = {232, 11, 13, 100, 255, 101, 200, 123, 55, 106, 72, 66, 66, 66, 1, 0};
        char s3[] = {"aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa"};
        /* TODO MORE? */
        assert(strlen(s1) == 11);
        assert(strlen(s2) == 15);
        assert(strlen(s3) == 73);
}

int main() {
        test_strlen();
        return 0;
}
