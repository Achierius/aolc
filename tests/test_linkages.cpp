#include "aolc/_test_string.h"
#include <string.h>
#include <signal.h>
#include <stdio.h>

#include "aolc/compare_buffer_functions.h"

#include "gtest/gtest.h"

char buff_a[] __attribute__ ((aligned (16))) = {'\0'};
char buff_b[] __attribute__ ((aligned (16))) = {'\0'};
char buff_strcat[] __attribute__ ((aligned (16))) = {'\0', '\0'};

TEST(memcpy, Linkage)
{
        _memcpy(buff_a, buff_b, 0);
}

TEST(memmove, Linkage)
{
        _memmove(buff_a, buff_b, 0);
}

TEST(memchr, Linkage)
{
        _memchr(buff_a, 0, 0);
}

TEST(memcmp, Linkage)
{
        _memcmp(buff_a, buff_b, 0);
}

TEST(memset, Linkage)
{
        _memset(buff_a, 0, 0);
}

TEST(strcat, Linkage)
{
        _strcat(buff_strcat, buff_b);
}

TEST(strncat, Linkage)
{
        _strncat(buff_a, buff_b, 0);
}

TEST(strchr, Linkage)
{
        _strchr(buff_a, 0);
}

TEST(strrchr, Linkage)
{
        _strrchr(buff_a, 0);
}

TEST(strcmp, Linkage)
{
        _strcmp(buff_a, buff_a);
}

TEST(strncmp, Linkage)
{
        _strncmp(buff_a, buff_b, 0);
}

TEST(strcoll, Linkage)
{
        _strcoll(buff_a, buff_b);
}

TEST(strcpy, Linkage)
{
        _strcpy(buff_a, buff_b);
}

TEST(strncpy, Linkage)
{
        _strncpy(buff_a, buff_b, 0);
}

TEST(strerror, Linkage)
{
        _strerror(0);
}

TEST(strspn, Linkage)
{
        _strspn(buff_a, buff_b);
}

TEST(strcspn, Linkage)
{
        _strcspn(buff_a, buff_b);
}

TEST(strlen, Linkage)
{
        _strlen(buff_a);
}

TEST(strpbrk, Linkage)
{
        _strpbrk(buff_a, buff_b);
}

TEST(strstr, Linkage)
{
        _strstr(buff_a, buff_b);
}

TEST(strtok, Linkage)
{
        _strtok(buff_a, buff_b);
}

TEST(strxfrm, Linkage)
{
        _strxfrm(buff_a, buff_b, 0);
}
