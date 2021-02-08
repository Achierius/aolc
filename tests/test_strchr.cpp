#include "aolc/_test_string.h"
#include <string.h>

#include "gtest/gtest.h"
#include "aolc/compare_buffer_functions.h"

void CompareBufferFuncEvalStrchr(const char* str, int chr, size_t len,
                                 const char* comment) {
    using std::placeholders::_1;

    auto test_wrapper = [=](const char* s) {
        return static_cast<const char*>(_strchr(s, chr));
    };

    auto true_wrapper = std::bind(
            static_cast<const char* (*)(const char*, int)>(strchr),
            _1, chr);

    SCOPED_TRACE(comment);
    CompareBufferFunctions<const char*, const char*>(
            test_wrapper, true_wrapper, str, len,
            EqualityMode::kBufferRelativeEquality);
}

TEST(strchr, Basic) {
    char str[] = {'x', 'x', 'x', 'X', 'y', 'X', 'X', '\0'};
    size_t len = strlen(str) + 1;

    CompareBufferFuncEvalStrchr(str, 'x', len,  "str, x");
    CompareBufferFuncEvalStrchr(str, 'X', len,  "str, X");
    CompareBufferFuncEvalStrchr(str, 'y', len,  "str, y");
    CompareBufferFuncEvalStrchr(str, 'Y', len,  "str, Y");
    CompareBufferFuncEvalStrchr(str, '\0', len, "str, \\0");
}

TEST(strchr, Meta) {
    CompareBufferFuncEvalStrchr("x", 'x', 1, "x, x");
    CompareBufferFuncEvalStrchr("X", 'X', 1, "x, x");
    CompareBufferFuncEvalStrchr("&", '&', 1, "x, x");
}
