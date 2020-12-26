#ifndef __AOLC_COMPARE_BUFFER_FUNCTIONS_H
#define __AOLC_COMPARE_BUFFER_FUNCTIONS_H

#include <functional>

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


using VoidBuffFn            = std::function<void* (      void*, const void*)>;

using ConstVoidBuffFn       = std::function<void* (const void*, const void*)>;

using SingleVoidBuffFn      = std::function<void* (      void*             )>;

using ConstSingleVoidBuffFn = std::function<void* (const void*             )>;

using CharBuffFn            = std::function<char* (      char*, const char*)>;

using ConstCharBuffFn       = std::function<char* (const char*, const char*)>;

using SingleCharBuffFn      = std::function<char* (      char*             )>;

using ConstSingleCharBuffFn = std::function<char* (const char*             )>;



//using Class2Fn = std::function<char* (char*, const char*, size_t n)>;

/* I did these all manually (rather than using templates) because I wasn't
 * confident I could use enable_if properly so as to avoid accidentally
 * violating the standard -- lots of tricky aliasing going on in these */

void CompareBufferFuncEval(VoidBuffFn test_func, VoidBuffFn true_func,
                                     const void* s1, const void* s2, size_t l1, size_t l2);

void CompareBufferFuncEval(SingleVoidBuffFn test_func, SingleVoidBuffFn true_func,
                                     const void* s1, size_t l1);

void CompareBufferFuncEval(SingleCharBuffFn test_func, SingleCharBuffFn true_func,
                                     const char* s1, size_t l1);

//void CompareBufferFuncEval(ConstSingleCharBuffFn test_func, ConstSingleCharBuffFn true_func,
//                                     const char* s1, size_t l1);


/* Convenience functions to simply call strlen on the buffers */

//void CompareBufferFuncEval(SingleCharBuffFn test_func, SingleCharBuffFn true_func,
//                                     const char* s1);

void CompareBufferFuncEval(ConstSingleCharBuffFn test_func, ConstSingleCharBuffFn true_func,
                                     const char* s1);

template<typename RetT, typename ArgT1, typename ArgT2>
void CompareBufferFunctions(std::function<RetT* (ArgT1*, ArgT2*)> test_func,
                            std::function<RetT* (ArgT1*, ArgT2*)> true_func,
                            const void* buff_1, const void* buff_2,
                            size_t l1, size_t l2) {
    auto test_func_wrapper = [=](void* s1, const void* s2) {
        ArgT1* arg1 = static_cast<ArgT1*>(s1);
        ArgT2* arg2 = static_cast<ArgT2*>(s2);
        return static_cast<RetT*>(test_func(arg1, arg2));
    };
    auto true_func_wrapper = [=](void* s1, const void* s2) {
        ArgT1* arg1 = static_cast<ArgT1*>(s1);
        ArgT2* arg2 = static_cast<ArgT2*>(s2);
        return static_cast<RetT*>(true_func(arg1, arg2));
    };

    CompareBufferFuncEval(test_func_wrapper, true_func_wrapper,
                          buff_1, buff_2, l1, l2);
}

template<typename RetT, typename ArgT1, typename ArgT2>
void CompareBufferFunctions(std::function<RetT* (ArgT1*, ArgT2*)> test_func,
                            std::function<RetT* (ArgT1*, ArgT2*)> true_func,
                            const void* buff_1, const void* buff_2) {
    CompareBufferFunctions<RetT, ArgT1, ArgT2>(
        test_func, true_func,
        buff_1, buff_2,
        strlen(static_cast<const char*>(buff_1)),
        strlen(static_cast<const char*>(buff_2)));
}

template<typename ArgT1, typename ArgT2>
void CompareComparisonFunctions(std::function<int (ArgT1*, ArgT2*)> test_func,
                                std::function<int (ArgT1*, ArgT2*)> true_func,
                                const void* buff_1, const void* buff_2) {
}
#endif//__AOLC_COMPARE_BUFFER_FUNCTIONS_H
