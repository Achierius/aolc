#include <string.h>
#include "aolc/_test_string.h"
#include "aolc/compare_buffer_functions.h"
#include "gtest/gtest.h"

#include <assert.h>
#include <stdio.h>
#include <functional>


TEST(memmove, Basic) {
    using std::placeholders::_1;
    using std::placeholders::_2;

    char s1[] = {'a', 'a', 'a', 'a', 'a', 's', 's', 's', 's', 's', 's', 'd', 'd', 'd', 'd',
                 'd', 'd', 'd', 'd', 'd', 'd', 'd'};
    char s2[] = {'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x',
                 'x', 'x', 'x', 'x', 'x', 'x', 'x'};
    char s3[] = ":)";

    auto CompareBufferFuncEvalMemmove = [&](const char* s1, const char* s2, size_t len, const char* comment) {
        auto true_fn = std::bind(memmove, _1, _2, len);
        auto test_fn = std::bind(_memmove, _1, _2, len);
        SCOPED_TRACE(comment);
        CompareBufferFuncEval(test_fn, true_fn, s1, s2, len, len);
    };

    CompareBufferFuncEvalMemmove(s1, s2, 20, "s1, s2, 20");
    CompareBufferFuncEvalMemmove(s2, s1, 22, "s2, s1, 22");
    CompareBufferFuncEvalMemmove(s3, s2, 2,  "s3, s2, 2");
    CompareBufferFuncEvalMemmove(s3, s1, 2,  "s3, s1, 2");
    CompareBufferFuncEvalMemmove(s1, s2, 4,  "s1, s2, 4");
}

TEST(memmove, Overlapping) {
    char s1[] = {'a', 'a', 'a', 'a', 'a', 's', 's', 's', 's', 's', 's', 'd', 'd', 'd', 'd', 'd', 'd', 'd', 'd', 'd', 'd', 'd'};
    char s2[] = {'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x'};

    auto CompareBufferFuncEvalMemmove = [&](const char* s1, const char* s2, size_t len, const char* comment) {
        auto true_fn = std::bind(memmove, _1, _2, len);
        auto test_fn = std::bind(_memmove, _1, _2, len);
        SCOPED_TRACE(comment);
        CompareBufferFuncEval(test_fn, true_fn, s1, s2, len, len);
    };

    CompareBufferFuncEvalMemmove(s1 + 0, s1 + 9, 20, "s1 + 0, s1 + 9, 20");
    CompareBufferFuncEvalMemmove(s2 + 8, s2 + 0, 10, "s2 + 8, s2 + 0, 10");
    CompareBufferFuncEvalMemmove(s1 + 1, s2 + 1, 25, "s1 + 1, s2 + 1, 25");
}
