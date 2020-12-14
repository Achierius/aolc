#ifndef __AOLC_COMPARE_BUFFER_FUNCTIONS_H
#define __AOLC_COMPARE_BUFFER_FUNCTIONS_H

/*
 * Class 0: (int)
 *     =>  strerror
 *         >char*
 *
 * Class 1: (char*)
 *     =>  strlen
 *         >size_t
 *
 * Class 2: (char*, char*)
 *     =>  strcat  strcpy  strpbrk  strstr  strtok  strspn  strcspn 
 *         >
 *
 * Class 3: (char*, char*, size_t)
 *     => strncmp  strncpy  strxfrm  memcpy  memmove  memcmp  strncat
 *
 * Class 4: (char*, int)
 *     =>  strchr  strrchr  strcmp  strcoll
 *
 * Class 5: (char*, int, size_t)
 *     =>  memchr  memset
 */

using Class2Fn  = char* (*)(char*, const char*);
using Class2nFn = char* (*)(char*, const char*, size_t);
using Class3Fn  = void* (*)(void*, const void*, size_t);
void evaluate_strfn(Class2Fn test_strfn, Class2Fn true_strfn, char* dst, char* src);
void evaluate_strnfn(Class2nFn test_strfn, Class2nFn true_strfn, char* dst, char* src, size_t n);
void evaluate_memfn(Class3Fn test_memfn, Class3Fn true_memfn, void* dst, void* src, size_t n);

#endif//__AOLC_COMPARE_BUFFER_FUNCTIONS_H
