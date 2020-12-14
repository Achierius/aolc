/* Proof that we can compile this thing correctly :) */

#include <string.h>
#include <stdio.h>

int main(int argc, char *argv[]) {
        char str[] = "this format string has length %d\n";
        printf(str, strlen(str));
        return 0;
}
