#include "aolc/_test_string.h"
#include <string.h>

#include "aolc/compare_buffer_functions.h"
#include "gtest/gtest.h"

TEST(memset, Basic) {
    using std::placeholders::_1;
    using std::placeholders::_2;

    char s_true[] = {"aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa"};
    
    auto CompareBufferFuncEvalMemset = [&](const void* dst, const int c, const size_t n, const char* comment) {
        auto true_fn = std::bind(memset, _1, c, n);
        auto test_fn = std::bind(_memset, _1, c, n);
        SCOPED_TRACE(comment);
        CompareBufferFuncEval(test_fn, true_fn, dst, n);
    };

    CompareBufferFuncEvalMemset(s_true + 3, ' ', 10, "s_true + 3, ' ', 10");
    CompareBufferFuncEvalMemset(s_true + 0, 'X', 20, "s_true + 0, 'X', 20");
    CompareBufferFuncEvalMemset(s_true + 13, '\0', 1, "s_true + 13, '\0', 1");
}
