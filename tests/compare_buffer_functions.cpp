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

template <typename T> int sgn(T val) {
    return (T(0) < val) - (val < T(0));
}

void CompareBufferFuncEval(VoidBuffFn test_func, VoidBuffFn true_func,
                           const void* s1, const void* s2,
                           size_t l1, size_t l2,
                           EqualityMode equality_mode) {
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
        if (true_retval == nullptr || test_retval == nullptr) {
            EXPECT_EQ(test_retval, nullptr);
            EXPECT_EQ(test_retval, true_retval);
        } else {
            EXPECT_EQ((intptr_t) true_retval - (intptr_t) true_s1, (intptr_t) test_retval - (intptr_t) test_s1);
        }
        break;
      case EqualityMode::kSignEquality:
        /* These functions (strcmp, memcmp) return an integer value, in which
         * the result of their computation is embedded via the sign -- positive
         * denoting arg2 > arg1, negative denoting arg2 < arg1, and 0 denoting
         * arg1 == arg2; beyond this, the value is not specified. */
        EXPECT_EQ(sgn<void*>(true_retval), sgn<void*>(test_retval));
        break;
    }

    /* Release global buffers */
    buffers_lock.unlock();
}
