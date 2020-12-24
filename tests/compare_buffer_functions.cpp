#include "gtest/gtest.h"
#include <stdio.h>
#include <cstring>
#include <mutex>
#include <iostream>
#include "aolc/_test_string.h"
#include "aolc/compare_buffer_functions.h"

#include <functional>

const size_t kMaxBufferSize = 4096;
const uint8_t kCanaryByte = 0xCA;
const size_t kCanaryLengthBytes = 4;

std::mutex buffers_lock;
uint8_t canary_lock[kCanaryLengthBytes] = {0};

uint8_t buffer_src1[kMaxBufferSize] = {0};
uint8_t canary_src1[kCanaryLengthBytes] = {0};

uint8_t buffer_src2[kMaxBufferSize] = {0};
uint8_t canary_src2[kCanaryLengthBytes] = {0};

uint8_t buffer_dst1[kMaxBufferSize] = {0};
uint8_t canary_dst1[kCanaryLengthBytes] = {0};

uint8_t buffer_dst2[kMaxBufferSize] = {0};
uint8_t canary_dst2[kCanaryLengthBytes] = {0};

void CleanBuffers() {
    memset(buffer_src1, '\0', kMaxBufferSize);
    memset(buffer_src2, '\0', kMaxBufferSize);
    memset(buffer_dst1, '\0', kMaxBufferSize);
    memset(buffer_dst2, '\0', kMaxBufferSize);
    memset(canary_lock, kCanaryByte, kCanaryLengthBytes);
    memset(canary_src2, kCanaryByte, kCanaryLengthBytes);
    memset(canary_src1, kCanaryByte, kCanaryLengthBytes);
    memset(canary_dst1, kCanaryByte, kCanaryLengthBytes);
    memset(canary_dst2, kCanaryByte, kCanaryLengthBytes);
}

void CheckCanary(uint8_t* canary) {
    for (int i = 0; i < kCanaryLengthBytes; i++) {
        ASSERT_EQ(canary[i], kCanaryByte);
    }
}

void CheckCanaries() {
    CheckCanary(canary_lock);
    CheckCanary(canary_src1);
    CheckCanary(canary_src2);
    CheckCanary(canary_dst1);
    CheckCanary(canary_dst2);
}


/* Wrapper for (void* (*)(const void*, const void*)) */
void CompareBufferFuncEval(ConstVoidBuffFn test_func, ConstVoidBuffFn true_func, const void* s1, const void* s2, size_t l1, size_t l2) {
    /* Standard compliant, as void* <= const void* */
    auto test_func_wrapper = [=](void* s1, const void* s2) { return test_func(s1, s2); };
    auto true_func_wrapper = [=](void* s1, const void* s2) { return true_func(s1, s2); };

    CompareBufferFuncEval(test_func_wrapper, true_func_wrapper, s1, s2, l1, l2);
}

/* Wrapper for (char* (*)(const char*, const char*)) */
void CompareBufferFuncEval(ConstCharBuffFn test_func, ConstCharBuffFn true_func, const char* s1, const char* s2, size_t l1, size_t l2) {
    /* Standard compliant, as char* <= const char* */
    auto test_func_wrapper = [=](char* s1, const char* s2) { return test_func(s1, s2); };
    auto true_func_wrapper = [=](char* s1, const char* s2) { return true_func(s1, s2); };

    CompareBufferFuncEval(test_func_wrapper, true_func_wrapper, s1, s2, l1, l2);
}

/* Wrapper for (char* (*)(char*, const char*)) */
void CompareBufferFuncEval(CharBuffFn test_func, CharBuffFn true_func,
                           const char* s1, const char* s2,
                           size_t l1, size_t l2) {

    /* Standard-compliant, as we're ultimately converting back into char* via
     * memcpy and such */
    auto test_func_wrapper = [=](void* s1, const void* s2) { return test_func((char*) s1, (const char*) s2); };
    auto true_func_wrapper = [=](void* s1, const void* s2) { return true_func((char*) s1, (const char*) s2); };

    CompareBufferFuncEval(test_func_wrapper, true_func_wrapper,
                                    static_cast<const void*>(s1), static_cast<const void*>(s2), l1, l2);
}

void CompareBufferFuncEval(ConstSingleVoidBuffFn test_func,
                                     ConstSingleVoidBuffFn true_func,
                                     const void* s1, size_t l1) {
    auto test_func_wrapper = [=](void* s1, const void* s2) { return test_func(s1); };
    auto true_func_wrapper = [=](void* s1, const void* s2) { return true_func(s1); };
    
    CompareBufferFuncEval(test_func_wrapper, true_func_wrapper,
                                    s1, nullptr, l1, 0);
}

void CompareBufferFuncEval(SingleVoidBuffFn test_func,
                                     SingleVoidBuffFn true_func,
                                     const void* s1, size_t l1) {
    auto test_func_wrapper = [=](void* s1, const void* s2) { return test_func(s1); };
    auto true_func_wrapper = [=](void* s1, const void* s2) { return true_func(s1); };
    
    CompareBufferFuncEval(test_func_wrapper, true_func_wrapper,
                                    s1, nullptr, l1, 0);
}

void CompareBufferFuncEval(ConstSingleCharBuffFn test_func,
                                     ConstSingleCharBuffFn true_func,
                                     const char* s1, size_t l1) {
    auto test_func_wrapper = [=](char* s1, const char* s2) { return test_func(s1); };
    auto true_func_wrapper = [=](char* s1, const char* s2) { return true_func(s1); };
    
    CompareBufferFuncEval(test_func_wrapper, true_func_wrapper,                                    s1, nullptr, l1, 0);
}

void CompareBufferFuncEval(SingleCharBuffFn test_func,
                                     SingleCharBuffFn true_func,
                                     const char* s1, size_t l1) {
    auto test_func_wrapper = [=](char* s1, const char* s2) { return test_func(s1); };
    auto true_func_wrapper = [=](char* s1, const char* s2) { return true_func(s1); };
    
    CompareBufferFuncEval(test_func_wrapper, true_func_wrapper,
                                    s1, nullptr, l1, 0);
}

void CompareBufferFuncEval(CharBuffFn test_func, CharBuffFn true_func, const char* s1, const char* s2) {
    CompareBufferFuncEval(test_func, true_func, s1, s2, strlen(s1), strlen(s2));
}

void CompareBufferFuncEval(ConstCharBuffFn test_func, ConstCharBuffFn true_func, const char* s1, const char* s2) {
    CompareBufferFuncEval(test_func, true_func, s1, s2, strlen(s1), strlen(s2));
}

void CompareBufferFuncEval(SingleCharBuffFn test_func, SingleCharBuffFn true_func, const char* s1) {
    CompareBufferFuncEval(test_func, true_func, s1, strlen(s1));
}

void CompareBufferFuncEval(ConstSingleCharBuffFn test_func, ConstSingleCharBuffFn true_func, const char* s1) {
    CompareBufferFuncEval(test_func, true_func, s1, strlen(s1));
}

void CompareBufferFuncEval(VoidBuffFn test_func, VoidBuffFn true_func,
                           const void* s1, const void* s2,
                           size_t l1, size_t l2) {
    ASSERT_LT(l1, kMaxBufferSize);
    ASSERT_LT(l2, kMaxBufferSize);

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
    void* true_retval = true_func(true_s1, true_s2);
    void* test_retval = test_func(test_s1, test_s2);

    /* Check to see if any stack canary has been overwritten -- this would
     * indicate a buffer overflow. Assuming we're not dealing with hostile code
     * -- libc <string.h> functions are by no means meant to be safe in this
     * regard -- this will catch all buffer overruns which write
     * 'continuously', that is without skipping large segments of memory as
     * they run out of bounds. */
    CheckCanaries();

    /* In general, these functions return either their *dst argument or a
     * pointer to a location within it -- as such, as long as neither function
     * errors out (by returning nullptr) we don't compare the actual return
     * values of each function, but rather the offset of their return values
     * from their *dst argument. */
    if (true_retval == nullptr || test_retval == nullptr) {
        EXPECT_EQ(test_retval, nullptr);
        EXPECT_EQ(test_retval, true_retval);
    } else {
        EXPECT_EQ((intptr_t) true_retval - (intptr_t) true_s1, (intptr_t) test_retval - (intptr_t) test_s1);
    }

    /* Now we compare the contents of both sets of buffers: the behavior of both
     * given functions is expected to be identical on each. */
    EXPECT_EQ(memcmp(test_s1, true_s1, kMaxBufferSize), 0);
    EXPECT_EQ(memcmp(test_s2, true_s2, kMaxBufferSize), 0);

    /* Release global buffers */
    buffers_lock.unlock();
}



//void CompareBufferComparisonFuncEval(???
