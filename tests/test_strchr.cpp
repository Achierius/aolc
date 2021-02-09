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

TEST(strchr, LongUnaligned) {
    const char* str = "--------===================A=B=C=D=E=F=G=H=";

    std::array<char, 8> toks = {'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H'};
    std::array<size_t, 8> offs = {0, 1, 2, 3, 4, 5, 6, 7};

    for (size_t off : offs) {
        for (char tok : toks) {
            CompareBufferFuncEvalStrchr(str + off, tok, strlen(str),
                                        ("off: " + std::to_string(off) + "; tok: " + std::to_string(tok)).c_str());
        }
    }
}

TEST(strchr, FalseFlags) {
    const char* str = "1********2********3********4********\0abcd1234";
    size_t str_len = strlen(str) + 10;

    /* None of these should succeed */
    CompareBufferFuncEvalStrchr(str, 'a', str_len, "str, 'a'");
    CompareBufferFuncEvalStrchr(str, 'b', str_len, "str, 'b'");
    CompareBufferFuncEvalStrchr(str, 'c', str_len, "str, 'c'");
    CompareBufferFuncEvalStrchr(str, 'd', str_len, "str, 'd'");

    /* All of these should succeed */
    CompareBufferFuncEvalStrchr(str, '1', str_len, "str, '1'");
    CompareBufferFuncEvalStrchr(str, '2', str_len, "str, '2'");
    CompareBufferFuncEvalStrchr(str, '3', str_len, "str, '3'");
    CompareBufferFuncEvalStrchr(str, '4', str_len, "str, '4'");
}
