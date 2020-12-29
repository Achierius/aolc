#include "aolc/_test_string.h"
#include <string.h>

#include "gtest/gtest.h"
#include "aolc/compare_buffer_functions.h"

void CompareMemchrEval(const void* buff, int c, size_t num,
                       const char* comment) {
    using std::placeholders::_1;

    auto test_wrapper = [=](const void* b) {
        return static_cast<const void*>(_memchr(b, c, num));
    };

    auto true_wrapper = std::bind(
            static_cast<const void* (*)(const void*, int, size_t)>(memchr),
            _1, c, num);

    SCOPED_TRACE(comment);
    CompareBufferFunctions<const void*, const void*>(
            test_wrapper, true_wrapper, buff, num,
            EqualityMode::kBufferRelativeEquality);
}

TEST(memchr, Basic) {
    char buff[] = {'x', 'x', 'x', 'X', 'y', 'X', 'X', '\0'};
    size_t len = strlen(buff) + 1;

    CompareMemchrEval(buff, 'x', len,  "buff, x");
    CompareMemchrEval(buff, 'X', len,  "buff, X");
    CompareMemchrEval(buff, 'y', len,  "buff, y");
    CompareMemchrEval(buff, 'Y', len,  "buff, Y");
    CompareMemchrEval(buff, '\0', len, "buff, \\0");
}

TEST(memchr, Meta) {
    CompareMemchrEval("x", 'x', 1, "x, x");
    CompareMemchrEval("X", 'X', 1, "x, x");
    CompareMemchrEval("&", '&', 1, "x, x");
}
