#include "gtest/gtest.h"
#include <stdio.h>
#include <cstring>
#include <mutex>
#include <iostream>
#include "aolc/_test_string.h"
#include "aolc/compare_buffer_functions.h"

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

void evaluate_strfn(Class2Fn test_strfn, Class2Fn true_strfn, char* dst, char* src)
{
    size_t src_len = strnlen(src, kMaxBufferSize) + 1;
    size_t dst_len = strnlen(dst, kMaxBufferSize) + 1;

    ASSERT_LT(src_len, kMaxBufferSize);
    ASSERT_LT(dst_len, kMaxBufferSize);

    /* Prepare to access global buffers */
    buffers_lock.lock();
    CleanBuffers();

    /* Copy given arguments into global buffers; we now have identical
     * duplicate buffers to pass as arguments to each function, so as
     * to properly compare their functionality. */
    memmove((char*) buffer_src1, src, src_len);
    memmove((char*) buffer_src2, src, src_len);
    memmove((char*) buffer_dst1, dst, dst_len);
    memmove((char*) buffer_dst2, dst, dst_len);

    /* Alias buffer names for clarity */
    char* true_src = (char*) buffer_src1;
    char* test_src = (char*) buffer_src2;
    char* true_dst = (char*) buffer_dst1;
    char* test_dst = (char*) buffer_dst2;

    /* Evaluate each of the given functions and store their return values for
     * later comparison */
    char* true_retval = true_strfn(true_dst, true_src);
    char* test_retval = test_strfn(test_dst, test_src);

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
        EXPECT_EQ((intptr_t) true_retval - (intptr_t) true_dst, (intptr_t) test_retval - (intptr_t) test_dst);
    }

    /* Now we compare the contents of both sets of buffers: the behavior of both
     * given functions is expected to be identical on each. */
    EXPECT_EQ(memcmp(test_dst, true_dst, kMaxBufferSize), 0);
    EXPECT_EQ(memcmp(test_src, true_src, kMaxBufferSize), 0);

    /* Release global buffers */
    buffers_lock.unlock();
}

void evaluate_strnfn(Class2nFn test_strfn, Class2nFn true_strfn, char* dst, char* src, size_t n)
{
    evaluate_memfn((Class3Fn) test_strfn, (Class3Fn) true_strfn, (void*) dst, (void*) src, n);
}

void evaluate_memfn(Class3Fn test_memfn, Class3Fn true_memfn, void* dst, void* src, size_t n)
{
    ASSERT_LT(n, kMaxBufferSize);

    /* Prepare to access global buffers */
    buffers_lock.lock();
    CleanBuffers();

    /* Copy given arguments into global buffers; we now have identical
     * duplicate buffers to pass as arguments to each function, so as
     * to properly compare their functionality. */
    memmove((char*) buffer_src1, src, n);
    memmove((char*) buffer_src2, src, n);
    memmove((char*) buffer_dst1, dst, n);
    memmove((char*) buffer_dst2, dst, n);

    /* Alias buffer names for clarity */
    void* true_src = buffer_src1;
    void* test_src = buffer_src2;
    void* true_dst = buffer_dst1;
    void* test_dst = buffer_dst2;

    /* Evaluate each of the given functions and store their return values for
     * later comparison */
    void* true_retval = true_memfn(true_dst, true_src, n);
    void* test_retval = test_memfn(test_dst, test_src, n);

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
        EXPECT_EQ((intptr_t) true_retval - (intptr_t) true_dst, (intptr_t) test_retval - (intptr_t) test_dst);
    }

    /* Now we compare the contents of both sets of buffers: the behavior of both
     * given functions is expected to be identical on each. */
    EXPECT_EQ(memcmp(test_dst, true_dst, n), 0);
    EXPECT_EQ(memcmp(test_src, true_src, n), 0);

    /* Release global buffers */
    buffers_lock.unlock();
}
