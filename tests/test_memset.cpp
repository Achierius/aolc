#include "aolc/_test_string.h"
#include <string.h>

#include "aolc/compare_buffer_functions.h"
#include "gtest/gtest.h"

void CompareMemsetEval(const void* dst, const int c, const size_t n, const char* comment) {
    using std::placeholders::_1;
    using std::placeholders::_2;

    auto true_fn = std::bind(memset, _1, c, n);
    auto test_fn = std::bind(_memset, _1, c, n);
    SCOPED_TRACE(comment);
    CompareBufferFunctions<void*, void*>(test_fn, true_fn, dst, n,
                                         EqualityMode::kBufferRelativeEquality);
};

TEST(memset, Basic) {
    char s_true[] = {"aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa"};
    
    CompareMemsetEval(s_true + 3, ' ', 10, "s_true + 3, ' ', 10");
    CompareMemsetEval(s_true + 0, 'X', 20, "s_true + 0, 'X', 20");
    CompareMemsetEval(s_true + 13, '\0', 1, "s_true + 13, '\0', 1");
}
