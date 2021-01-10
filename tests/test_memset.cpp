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

TEST(memset, Manual) {
    const bool kDebugPrint = false;
    const size_t buff_len = 78;

    char* true_buff = new char[buff_len];
    char* test_buff = new char[buff_len];
    memset(true_buff, '\0', buff_len);
    memset(test_buff, '\0', buff_len);


    void* true_rv =  memset(true_buff, 'x', 15);
    memset(true_buff + 13, 'y', 6);
    memset(true_buff + 24, 'z', 5);
    memset(true_buff + 32, 'W', 8);
    memset(true_buff + 40, 'A', 9);

    void* test_rv = _memset(test_buff, 'x', 15);
    _memset(test_buff + 13, 'y', 6);
    _memset(test_buff + 24, 'z', 5);
    _memset(test_buff + 32, 'W', 8);
    _memset(test_buff + 40, 'A', 9);

    if (kDebugPrint) {
        printf("true buffer: %s\n", true_buff);
        printf("test buffer: %s\n", test_buff);
    }

    for (int i = 0; i < buff_len; i++) {
        EXPECT_EQ(true_buff[i], test_buff[i]);
    }

    EXPECT_EQ((uintptr_t) true_rv - (uintptr_t) true_buff, (uintptr_t) test_rv - (uintptr_t) test_buff);
    if (kDebugPrint) {
        printf("true buffer: ");
        for (int i = 0; i < buff_len/8; i++) {
            for (int j = 0; j < 8; j++) {
                int index = 8*i + j;
                char chr = true_buff[index];
                if (chr == '\0') {
                    chr = '#';
                }
                printf("%c", chr);
                printf("\u001b[0m");
            }
            printf(" ");
        }
        printf("\n");

        printf("test buffer: ");
        for (int i = 0; i < buff_len/8; i++) {
            for (int j = 0; j < 8; j++) {
                int index = 8*i + j;
                char chr = test_buff[index];
                if (true_buff[index] != test_buff[index]) {
                    printf("\u001b[31m");
                }
                if (chr == '\0') {
                    chr = '#';
                }
                printf("%c", chr);
                printf("\u001b[0m");
            }
            printf(" ");
        }
        printf("\n");
    }
}
