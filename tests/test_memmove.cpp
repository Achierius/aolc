#include <string.h>
#include "aolc/_test_string.h"
#include "aolc/compare_buffer_functions.h"
#include "gtest/gtest.h"

#include <assert.h>
#include <stdio.h>
#include <functional>

void CompareMemmoveEval(const char* s1, const char* s2,
                        size_t len, const char* comment) {
    using std::placeholders::_1;
    using std::placeholders::_2;

    auto true_fn = std::bind(memmove, _1, _2, len);
    auto test_fn = std::bind(_memmove, _1, _2, len);
    SCOPED_TRACE(comment);
    CompareBufferFunctions<void*, void*, const void*>(
        test_fn, true_fn, s1, s2, len, len, EqualityMode::kBufferRelativeEquality);
}

TEST(memmove, Basic) {

    char s1[] = {'a', 'a', 'a', 'a', 'a', 's', 's', 's',
                 's', 's', 's', 'd', 'd', 'd', 'd', 'd',
                 'd', 'd', 'd', 'd', 'd', 'd'};
    char s2[] = {'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x',
                 'x', 'x', 'x', 'x', 'x', 'x', 'x',
                 'x', 'x', 'x', 'x', 'x', 'x', 'x'};
    char s3[] = ":)";

    CompareMemmoveEval(s1, s2, 20, "s1, s2, 20");
    CompareMemmoveEval(s2, s1, 22, "s2, s1, 22");
    CompareMemmoveEval(s3, s2, 2,  "s3, s2, 2");
    CompareMemmoveEval(s3, s1, 2,  "s3, s1, 2");
    CompareMemmoveEval(s1, s2, 4,  "s1, s2, 4");
}

TEST(memmove, Overlapping) {

    char s1[] = {'a', 'a', 'a', 'a', 'a', 's', 's', 's', 's', 's', 's', 'd', 'd', 'd', 'd', 'd', 'd', 'd', 'd', 'd', 'd', 'd'};
    char s2[] = {'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x'};

    CompareMemmoveEval(s1 + 0, s1 + 9, 20, "s1 + 0, s1 + 9, 20");
    CompareMemmoveEval(s2 + 8, s2 + 0, 10, "s2 + 8, s2 + 0, 10");
    CompareMemmoveEval(s1 + 1, s2 + 1, 25, "s1 + 1, s2 + 1, 25");
}
