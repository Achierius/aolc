#ifndef __AOLC_COMPARE_BUFFER_FUNCTIONS_H
#define __AOLC_COMPARE_BUFFER_FUNCTIONS_H

using Class2Fn  = char* (*)(char*, const char*);
using Class2nFn = char* (*)(char*, const char*, size_t);
using Class3Fn  = void* (*)(void*, const void*, size_t);
void evaluate_strfn(Class2Fn test_strfn, Class2Fn true_strfn, char* dst, char* src);
void evaluate_strnfn(Class2nFn test_strfn, Class2nFn true_strfn, char* dst, char* src, size_t n);
void evaluate_memfn(Class3Fn test_memfn, Class3Fn true_memfn, void* dst, void* src, size_t n);

#endif//__AOLC_COMPARE_BUFFER_FUNCTIONS_H
