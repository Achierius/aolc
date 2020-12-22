#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
int main() {
    extern void * _memmove(void *, void *, size_t);

    char * a = (char *) calloc(10, sizeof(char));
    char * b = (char *) calloc(10, sizeof(char));

    for (int i = 0; i < 10; ++i) {
        a[i] = 'a' + i;
        b[i] = 'z' - i;
    }

    for (int i = 0; i < 10; ++i) {
        printf("%c\n", a[i]);
        printf("%c\n", b[i]);
    }

    _memmove(a, b, 10);

    for (int i = 0; i < 10; ++i) {
        printf("%c\n", a[i]);
        printf("%c\n", b[i]);
    }

    free(b);
    free(a);
    return 1;
}
