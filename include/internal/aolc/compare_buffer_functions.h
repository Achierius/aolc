#ifndef __AOLC_COMPARE_BUFFER_FUNCTIONS_H
#define __AOLC_COMPARE_BUFFER_FUNCTIONS_H

#include "gtest/gtest.h"
#include <stdio.h>
#include <cstring>
#include <mutex>
#include <iostream>
#include "aolc/_test_string.h"
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


enum class EqualityMode {
    kStrictEquality,
    kBufferRelativeEquality,
    kSignEquality,
};

const size_t kMaxBufferSize = 4096;
const char kCanaryByte = 0xCA;
const size_t kCanaryLengthBytes = 4;

template <typename T> int sgn(T val) {
    return (T(0) < val) - (val < T(0));
}


//using Class2Fn = std::function<char* (char*, const char*, size_t n)>;

/* I did these all manually (rather than using templates) because I wasn't
 * confident I could use enable_if properly so as to avoid accidentally
 * violating the standard -- lots of tricky aliasing going on in these */

template<typename RetT>
void CompareBufferFuncEval(std::function<RetT (void*, const void*)> test_func,
                           std::function<RetT (void*, const void*)> true_func,
                           const void* s1, const void* s2,
                           size_t l1, size_t l2,
                           EqualityMode equality_mode) {

    std::mutex buffers_lock;
    char* canary_lock = (char*) calloc(1, kCanaryLengthBytes);

    char* buffer_src1 = (char*) calloc(1, kMaxBufferSize);
    char* canary_src1 = (char*) calloc(1, kCanaryLengthBytes);

    char* buffer_src2 = (char*) calloc(1, kMaxBufferSize);
    char* canary_src2 = (char*) calloc(1, kCanaryLengthBytes);

    char* buffer_dst1 = (char*) calloc(1, kMaxBufferSize);
    char* canary_dst1 = (char*) calloc(1, kCanaryLengthBytes);

    char* buffer_dst2 = (char*) calloc(1, kMaxBufferSize);
    char* canary_dst2 = (char*) calloc(1, kCanaryLengthBytes);

    ASSERT_LT(l1, kMaxBufferSize);
    ASSERT_LT(l2, kMaxBufferSize);

    auto CheckCanary = [&](char* canary) {
        for (int i = 0; i < kCanaryLengthBytes; i++) {
            ASSERT_EQ(canary[i], kCanaryByte);
        }
    };

    auto CheckCanaries = [&]() {
        CheckCanary(canary_lock);
        CheckCanary(canary_src1);
        CheckCanary(canary_src2);
        CheckCanary(canary_dst1);
        CheckCanary(canary_dst2);
    };

    auto CleanBuffers = [&]() {
        memset(buffer_src1, '\0', kMaxBufferSize);
        memset(buffer_src2, '\0', kMaxBufferSize);
        memset(buffer_dst1, '\0', kMaxBufferSize);
        memset(buffer_dst2, '\0', kMaxBufferSize);
        memset(canary_lock, kCanaryByte, kCanaryLengthBytes);
        memset(canary_src2, kCanaryByte, kCanaryLengthBytes);
        memset(canary_src1, kCanaryByte, kCanaryLengthBytes);
        memset(canary_dst1, kCanaryByte, kCanaryLengthBytes);
        memset(canary_dst2, kCanaryByte, kCanaryLengthBytes);
    };

    /* Prepare to access global buffers */
    buffers_lock.lock();
    CleanBuffers();

    /* Copy given arguments into global buffers; we now have identical
     * duplicate buffers to pass as arguments to each function, so as
     * to properly compare their functionality. */
    memmove(buffer_dst1, s1, l1);
    memmove(buffer_dst2, s1, l1);
    memmove(buffer_src1, s2, l2);
    memmove(buffer_src2, s2, l2);

    /* Alias buffer names for clarity */
    void* true_s1 = buffer_dst1;
    void* test_s1 = buffer_dst2;
    void* true_s2 = buffer_src1;
    void* test_s2 = buffer_src2;

    /* Evaluate each of the given functions and store their return values for
     * later comparison */
    RetT true_retval = true_func(true_s1, true_s2);
    RetT test_retval = test_func(test_s1, test_s2);

    /* Check to see if any stack canary has been overwritten -- this would
     * indicate a buffer overflow. Assuming we're not dealing with hostile code
     * -- libc <string.h> functions are by no means meant to be safe in this
     * regard -- this will catch all buffer overruns which write
     * 'continuously', that is without skipping large segments of memory as
     * they run out of bounds. */
    CheckCanaries();

    /* Now we compare the contents of both sets of buffers: the behavior of both
     * given functions is expected to be identical on each. */
    EXPECT_EQ(memcmp(test_s1, true_s1, kMaxBufferSize), 0);
    EXPECT_EQ(memcmp(test_s2, true_s2, kMaxBufferSize), 0);

    switch (equality_mode) {
      case EqualityMode::kStrictEquality:
        EXPECT_EQ(test_retval, true_retval);
        break;
      case EqualityMode::kBufferRelativeEquality:
        /* In general, these functions return either their *dst argument or a
         * pointer to a location within it -- as such, as long as neither function
         * errors out (by returning nullptr) we don't compare the actual return
         * values of each function, but rather the offset of their return values
         * from their *dst argument. */
        if (true_retval == static_cast<RetT>(0)) {
            EXPECT_EQ(test_retval, static_cast<RetT>(0));
            EXPECT_EQ(test_retval, true_retval);
        } else {
            EXPECT_EQ((uintptr_t) true_retval - (uintptr_t) true_s1, (uintptr_t) test_retval - (uintptr_t) test_s1);
        }
        EXPECT_EQ((intptr_t) true_retval - (intptr_t) true_s1, (intptr_t) test_retval - (intptr_t) test_s1);
        break;
      case EqualityMode::kSignEquality:
        /* These functions (strcmp, memcmp) return an integer value, in which
         * the result of their computation is embedded via the sign -- positive
         * denoting arg2 > arg1, negative denoting arg2 < arg1, and 0 denoting
         * arg1 == arg2; beyond this, the value is not specified. */
        EXPECT_EQ(sgn<RetT>(true_retval), sgn<RetT>(test_retval));
        break;
    }

    /* Release global buffers */
    buffers_lock.unlock();
}

template<typename RetT, typename ArgT1, typename ArgT2>
void CompareBufferFunctions(std::function<RetT (ArgT1, ArgT2)> test_func,
                            std::function<RetT (ArgT1, ArgT2)> true_func,
                            const void* buff_1, const void* buff_2,
                            size_t l1, size_t l2,
                            EqualityMode equality_mode =
                              EqualityMode::kBufferRelativeEquality) {
    auto test_func_wrapper = [=](void* s1, const void* s2) {
        ArgT1 arg1 = static_cast<ArgT1>(s1);
        ArgT2 arg2 = static_cast<ArgT2>(s2);
        return static_cast<RetT>(test_func(arg1, arg2));
    };
    auto true_func_wrapper = [=](void* s1, const void* s2) {
        ArgT1 arg1 = static_cast<ArgT1>(s1);
        ArgT2 arg2 = static_cast<ArgT2>(s2);
        return static_cast<RetT>(true_func(arg1, arg2));
    };

    CompareBufferFuncEval<RetT>(test_func_wrapper, true_func_wrapper,
                                buff_1, buff_2, l1, l2,
                                equality_mode);
}

template<typename RetT, typename ArgT1, typename ArgT2>
void CompareBufferFunctions(std::function<RetT (ArgT1, ArgT2)> test_func,
                            std::function<RetT (ArgT1, ArgT2)> true_func,
                            const void* buff_1, const void* buff_2,
                            EqualityMode equality_mode =
                              EqualityMode::kBufferRelativeEquality) {
    CompareBufferFunctions<RetT, ArgT1, ArgT2>(
        test_func, true_func,
        buff_1, buff_2,
        strlen(static_cast<const char*>(buff_1)),
        strlen(static_cast<const char*>(buff_2)),
        equality_mode);
}

template<typename RetT, typename ArgT1>
void CompareBufferFunctions(std::function<RetT (ArgT1)> test_func,
                            std::function<RetT (ArgT1)> true_func,
                            const void* buff_1,
                            size_t l1,
                            EqualityMode equality_mode =
                              EqualityMode::kBufferRelativeEquality) {
    auto test_func_wrapper = [=](ArgT1 s1, const void* s2) {
        return test_func(s1);
    };
    auto true_func_wrapper = [=](ArgT1 s1, const void* s2) {
        return true_func(s1);
    };

    CompareBufferFunctions<RetT, ArgT1, const void*>(
      test_func_wrapper, true_func_wrapper,
      buff_1, nullptr, l1, 0,
      equality_mode);
}

#endif//__AOLC_COMPARE_BUFFER_FUNCTIONS_H
