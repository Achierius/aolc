#ifndef __AOLC_STRING_H
#define __AOLC_STRING_H

#include <stddef.h>

#ifdef __cplusplus
extern "C"
{
#endif

#ifdef __AOLC_STREXT__GNU
/* GNU */
void *mempcpy(void *dest, const void *src, size_t n);

/* This avoids double-declaring `strerror_r` w/ both GNU & POSIX extns. enabled */
#ifndef __AOLC_STREXT__POSIX
/* GNU */
char *strerror_r(int, char *, size_t);
#endif

#endif /* __AOLC_STREXT__GNU */


#ifdef __AOLC_STREXT__ISO_IEC
/* ISO/IEC WDTR 24731 */
errno_t strcat_s(char *dest, size_t n, const char *src);

/* ISO/IEC WDTR 24731 */
errno_t strcpy_s(char *dest, size_t n, const char *src);
#endif /* __AOLC_STREXT__ISO_IEC */


#ifdef __AOLC_STREXT__BSD
/* OpenBSD, FreeBSD */
size_t strlcat(char *dest, const char *src, size_t n);

/* OpenBSD, FreeBSD */
size_t strlcpy(char *dest, const char *src, size_t n);

/* BSD, POSIX */
char *strdup(const char *src);
#endif /* __AOLC_STREXT__BSD */


#ifdef __AOLC_STREXT__POSIX

/* This avoids double-declaring `strdup` w/ both BSD & POSIX extns. enabled */
#ifndef __AOLC_STREXT__BSD
/* BSD, POSIX */
char *strdup(const char *src);
#endif

/* POSIX:2008 */
char *strsignal(int sig);

/* POSIX:2001 */
int strerror_r(int, char *, size_t);

/* POSIX */
char *strtok_r(char *, const char *delim, char **saveptr);

/* POSIX */
void *memccpy(void *dest, const void *src, int c, size_t n);

#endif /* __AOLC_STREXT__POSIX */

#ifdef __cplusplus
}
#endif

#endif /* __AOLC_STRING_H */
