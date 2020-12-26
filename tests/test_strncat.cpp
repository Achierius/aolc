#include "aolc/_test_string.h"
#include <string.h>

#include "aolc/compare_buffer_functions.h"
#include "gtest/gtest.h"

void CompareStrncpyEval(const char* s1,
                        const char* s2,
                        size_t n,
                        const char* comment) {
    using std::placeholders::_1;
    using std::placeholders::_2;

    auto true_fn = std::bind(strncpy, _1, _2, n);
    auto test_fn = std::bind(_strncpy, _1, _2, n);
    SCOPED_TRACE(comment);
    CompareBufferFunctions(test_fn, true_fn, s1, s2);
}

TEST(strncpy, Basic) {
    const char dest[] = {'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', '\0'};
    const char src_nonull[] = {'x', 'x', 'x', 'X', 'X', '\0'};
    const char src_yesnull[] = {'y', 'y', 'y', '\0'};
    const char goal[] = {'x', 'x', 'y', 'y', 'y', '\0', 'g', 'h', 'i', 'j', '\0'};

    CompareStrncpyEval(dest, src_nonull, 3, "dest, src_nonull, 3");
    CompareStrncpyEval(dest + 5, src_yesnull, 3, "dest + 5, src_yesnull, 3");
    CompareStrncpyEval(dest, src_yesnull, 3, "dest, src_yesnull, 8");
}
