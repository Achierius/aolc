#include <aolc/_test_string.h>

#include <string.h>

void *_memcpy(void *dest, const void *src, size_t n)
{
        return memcpy(dest, src, n);
}

void *_memmove(void *dest, const void *src, size_t n)
{
        return memmove(dest, src, n);
}

void *_memchr(const void *str, int c, size_t n)
{
        return memchr(str, c, n);
}

int _memcmp(const void *str1, const void *str2, size_t n)
{
        return memcmp(str1, str2, n);
}

void *_memset(void *str, int c, size_t n)
{
        return memset(str, c, n);
}

char *_strcat(char *dest, const char *src)
{
        return strcat(dest, src);
}

char *_strncat(char *dest, const char *src, size_t n)
{
        return strncat(dest, src, n);
}

char *_strchr(const char *str, int c)
{
        return strchr(str, c);
}

char *_strrchr(const char *str, int c)
{
        return strrchr(str, c);
}

int _strcmp(const char *str1, const char *str2)
{
        return strcmp(str1, str2);
}

int _strncmp(const char *str1, const char *str2, size_t n)
{
        return strncmp(str1, str2, n);
}

int _strcoll(const char *str1, const char *str2)
{
        return strcoll(str1, str2);
}

char *_strcpy(char *dest, const char *src)
{
        return strcpy(dest, src);
}

char *_strncpy(char *dest, const char *src, size_t n)
{
        return strncpy(dest, src, n);
}

char *_strerror(int errnum)
{
        return strerror(errnum);
}

size_t _strspn(const char *str1, const char *accept)
{
        return strspn(str1, accept);
}

size_t _strcspn(const char *str1, const char *reject)
{
        return strcspn(str1, reject);
}

size_t _strlen(const char *str)
{
        return strlen(str);
}

char *_strpbrk(const char *str1, const char *str2)
{
        return strpbrk(str1, str2);
}

char *_strstr(const char *haystack, const char *needle)
{
        return strstr(haystack, needle);
}

char *_strtok(char *str, const char *delim)
{
        return strtok(str, delim);
}

size_t _strxfrm(char *dest, const char *src, size_t n)
{
        return strxfrm(dest, src, n);
}
