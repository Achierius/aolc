#include "aolc/_test_string.h"
#include <string.h>

#include "gtest/gtest.h"
#include "aolc/compare_buffer_functions.h"

void CompareBufferFuncEvalStrrchr(const char* str, int chr, size_t len,
                                 const char* comment) {
    using std::placeholders::_1;

    auto test_wrapper = [=](const char* s) {
        return static_cast<const char*>(_strrchr(s, chr));
    };

    auto true_wrapper = std::bind(
            static_cast<const char* (*)(const char*, int)>(strrchr),
            _1, chr);

    SCOPED_TRACE(comment);
    CompareBufferFunctions<const char*, const char*>(
            test_wrapper, true_wrapper, str, len,
            EqualityMode::kBufferRelativeEquality);
}

TEST(strrchr, Basic) {
    char str[] = {'x', 'x', 'x', 'X', 'y', 'X', 'X', '\0'};
    size_t len = strlen(str) + 1;

    CompareBufferFuncEvalStrrchr(str, 'x', len,  "str, x");
    CompareBufferFuncEvalStrrchr(str, 'X', len,  "str, X");
    CompareBufferFuncEvalStrrchr(str, 'y', len,  "str, y");
    CompareBufferFuncEvalStrrchr(str, 'Y', len,  "str, Y");
    CompareBufferFuncEvalStrrchr(str, '\0', len, "str, \\0");
}

TEST(strrchr, Meta) {
    CompareBufferFuncEvalStrrchr("x", 'x', 1, "x, x");
    CompareBufferFuncEvalStrrchr("X", 'X', 1, "x, x");
    CompareBufferFuncEvalStrrchr("&", '&', 1, "x, x");
}
