#include "aolc/_test_string.h"
#include <string.h>

#include "aolc/compare_buffer_functions.h"
#include "gtest/gtest.h"

#include <functional>

TEST(strncpy, Basic)
{
    using std::placeholders::_1;
    using std::placeholders::_2;

    char dest[]        = {'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', '\0'};
    char src_nonull[]  = {'x', 'x', 'x', 'X', 'X', '\0'};
    char src_yesnull[] = {'y', 'y', 'y', '\0'};
    char goal[]        = {'x', 'x', 'x', 'd', 'e', 'y', 'y', 'y', '\0', '\0', '\0'};

    auto CompareBufferFuncEvalStrncpy = [&](const char* s1, const char* s2, size_t len, const char* comment) {
        auto true_fn = std::bind(strncpy, _1, _2, len);
        auto test_fn = std::bind(_strncpy, _1, _2, len);
        SCOPED_TRACE(comment);
        CompareBufferFuncEval(test_fn, true_fn, s1, s2, len, len);
    };

    CompareBufferFuncEvalStrncpy(dest, src_nonull, 3, "dest, src_nonull, 3");
    CompareBufferFuncEvalStrncpy(dest + 5, src_yesnull, 3, "dest + 5, src_yesnull, 3");
    CompareBufferFuncEvalStrncpy(goal, src_yesnull, 8, "goal, src_yesnull, 8");
}
