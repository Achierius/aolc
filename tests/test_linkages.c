#include "string.h"

/*
 * Placeholder until we include a proper testing framework.
 */

void test_linkages()
{
        memcpy(NULL, NULL, 0);

        memmove(NULL, NULL, 0);

        memchr(NULL, 0, 0);

        memcmp(NULL, NULL, 0);

        memset(NULL, 0, 0);

        strcat(NULL, NULL);

        strncat(NULL, NULL, 0);

        strchr(NULL, 0);

        strrchr(NULL, 0);

        strcmp(NULL, NULL);

        strncmp(NULL, NULL, 0);

        strcoll(NULL, NULL);

        strcpy(NULL, NULL);

        strncpy(NULL, NULL, 0);

        strerror(0);

        strspn(NULL, NULL);

        strcspn(NULL, NULL);

        strlen(NULL);

        strpbrk(NULL, NULL);

        strstr(NULL, NULL);

        strtok(NULL, NULL);

        strxfrm(NULL, NULL, 0);
}

int main()
{
        test_linkages();
}
