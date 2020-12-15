#ifndef __AOLC_STRING_H
#define __AOLC_STRING_H

#include <stddef.h>

#ifdef __cplusplus
extern "C"
{
#endif


void *_memcpy(void *dest, const void *src, size_t n);

void *_memmove(void *dest, const void *src, size_t n);

void *_memchr(const void *str, int c, size_t n);

int _memcmp(const void *str1, const void *str2, size_t n);

void *_memset(void *str, int c, size_t n);

char *_strcat(char *dest, const char *src);

char *_strncat(char *dest, const char *src, size_t n);

char *_strchr(const char *str, int c);

char *_strrchr(const char *str, int c);

int _strcmp(const char *str1, const char *str2);

int _strncmp(const char *str1, const char *str2, size_t n);

int _strcoll(const char *str1, const char *str2);

char *_strcpy(char *dest, const char *src);

char *_strncpy(char *dest, const char *src, size_t n);

char *_strerror(int errnum);

size_t _strspn(const char *str1, const char *accept);

size_t _strcspn(const char *str1, const char *reject);

size_t _strlen(const char *str);

char *_strpbrk(const char *str1, const char *str2);

char *_strstr(const char *haystack, const char *needle);

char *_strtok(char *str, const char *delim);

size_t _strxfrm(char *dest, const char *src, size_t n);

#ifdef __cplusplus
}
#endif

#endif//__AOLC_STRING_H
