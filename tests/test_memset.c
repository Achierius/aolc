#include "string.h"
#include <stdio.h>
#include <assert.h>


void test_memset() {
        extern size_t __strlen();

        char s[] = {"aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa"};
        char z[] = {"asd"};
        /* TODO MORE? */
        assert(memset(s + 4, 10, 3) == s + 4);
        assert(s[0] == 'a');
        assert(s[1] == 'a');
        assert(s[2] == 'a');
        assert(s[3] == 'a');
        assert(s[4] == 10);
        assert(s[5] == 10);
        assert(s[6] == 10);
        assert(s[7] == 'a');
        assert(memset(z, 'x', 2) == z);
        assert(s[0] == 'a');
        assert(s[1] == 'a');
        assert(s[2] == 'a');
        assert(s[3] == 'a');
        assert(s[4] == 10);
        assert(s[5] == 10);
        assert(s[6] == 10);
        assert(s[7] == 'a');
        assert(z[0] == 'x');
        assert(z[1] == 'x');
        assert(z[2] == 'd');
        memset(s + 15, 0, 1);
        assert(s[15] == 0);
}

int main() {
        test_memset();
        return 0;
}
