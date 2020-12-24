#include "aolc/_test_string.h"
#include <string.h>
#include <stddef.h>

#include "aolc/compare_buffer_functions.h"
#include "gtest/gtest.h"

#include <functional>

TEST(strcpy, Basic)
{
    using std::placeholders::_1;
    using std::placeholders::_2;

    const char dest[] = {'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', '\0'};
    const char src_nonull[] = {'x', 'x', 'x', 'X', 'X', '\0'};
    const char src_yesnull[] = {'y', 'y', 'y', '\0'};
    const char goal[] = {'x', 'x', 'y', 'y', 'y', '\0', 'g', 'h', 'i', 'j', '\0'};


    auto CompareBufferFuncEvalStrcpy = [&](const char* s1, const char* s2, const char* comment) {
        auto true_fn = std::bind(strcpy, _1, _2);
        auto test_fn = std::bind(_strcpy, _1, _2);
        SCOPED_TRACE(comment);
        CompareBufferFuncEval(test_fn, true_fn, s1, s2);
    };

    CompareBufferFuncEvalStrcpy(dest, src_nonull, "dest, src-nonull");
    CompareBufferFuncEvalStrcpy(dest, src_yesnull, "dest, yesnull");
}
