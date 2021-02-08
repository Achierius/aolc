#include <string.h>
#include "aolc/_test_string.h"
#include "aolc/compare_buffer_functions.h"
#include "gtest/gtest.h"

#include <assert.h>
#include <stdio.h>


void CompareMemcpyEval(const char* s1, const char* s2, size_t len, const char* comment) {
    using std::placeholders::_1;
    using std::placeholders::_2;

    auto true_fn = std::bind(memcpy, _1, _2, len);
    auto test_fn = std::bind(_memcpy, _1, _2, len);
    SCOPED_TRACE(comment);
    CompareBufferFunctions<void*, void*, const void*>(
        test_fn, true_fn, s1, s2, len, len,
        EqualityMode::kBufferRelativeEquality);
}

TEST(memcpy, Basic) {
    char s1[] = {'a', 'a', 'a', 'a', 'a', 's', 's', 's', 's', 's', 's', 'd', 'd', 'd', 'd',
                 'd', 'd', 'd', 'd', 'd', 'd', 'd'};
    char s2[] = {'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x',
                 'x', 'x', 'x', 'x', 'x', 'x', 'x'};
    char s3[] = ":)";

    CompareMemcpyEval(s1, s2, 20, "s1, s2, 20");
    CompareMemcpyEval(s2, s1, 22, "s2, s1, 22");
    CompareMemcpyEval(s3, s2, 2,  "s3, s2, 2");
    CompareMemcpyEval(s3, s1, 2,  "s3, s1, 2");
    CompareMemcpyEval(s1, s2, 4,  "s1, s2, 4");
}

TEST(memcpy, Length) {
    int* buff_src = new int[128];
    int* buff_dst = new int[256];

    for (int i = 0; i < 256; i++) {
        buff_dst[i] = 256 - i;
    }
    for (int i = 0; i < 128; i++) {
        buff_src[i] = i;
    }
    buff_src[20] = '\n'; // Don't get tripped up by null bytes :)

    CompareMemcpyEval((char*) buff_dst, (char*) buff_src, 4*4,   "4");
    CompareMemcpyEval((char*) buff_dst, (char*) buff_src, 4*8,   "8");
    CompareMemcpyEval((char*) buff_dst, (char*) buff_src, 4*16,  "16");
    CompareMemcpyEval((char*) buff_dst, (char*) buff_src, 4*32,  "32");
    CompareMemcpyEval((char*) buff_dst, (char*) buff_src, 4*64,  "64");
    CompareMemcpyEval((char*) buff_dst, (char*) buff_src, 4*128, "128");
}
