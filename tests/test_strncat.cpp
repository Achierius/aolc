#include "aolc/_test_string.h"
#include <string.h>

#include "aolc/compare_buffer_functions.h"
#include "gtest/gtest.h"

void CompareStrncatEval(const char* s1, const char* s2,
                        size_t n, const char* comment) {
    using std::placeholders::_1;
    using std::placeholders::_2;

    auto test_wrapper = std::bind(_strncat, _1, _2, n);
    auto true_wrapper = std::bind(strncat,  _1, _2, n);

    SCOPED_TRACE(comment);
    CompareBufferFunctions<char*, char*, const char*>(
            test_wrapper, true_wrapper, s1, s2,
            EqualityMode::kBufferRelativeEquality);
}

/* TODO actually test the 'n' part of str'n'cat */
// ^include n=0 buffer overflow case

TEST(strncat, Basic)
{
        char* buff = new char[16];
        char* b_0  = buff;
        char* b_5  = buff + 5;
        char* b_6  = buff + 6;
        char* b_10 = buff + 10;
        char* b_12 = buff + 12;
        char* b_14 = buff + 14;

        memcpy(buff, "xxxx\0\0\0\0\0aabb  ", 16);
        //            ^b_0  ^b_6    ^b_10
        //                ^b_5        ^b_12
        //                              ^b_14
        
        // Goal: "xxxxxxxx\0aabb  \0"
        CompareStrncatEval(b_0, b_0, 4, "b_0, b_0");
        // Goal: "xxxx  \0\0\0aabb  \0"
        CompareStrncatEval(b_0, b_10, 100, "b_0, b_14");
        // Goal: "xxxx\0\0\0\0\0aabb  \0"
        CompareStrncatEval(b_0, b_5,  100, "b_0, b_5");
        // Goal: "xxxxbb  \0\0\0aabb  \0"
        CompareStrncatEval(b_0, b_12, 4, "b_0, b_12");
        // Goal: "xxxxbb  \0\0\0aabb  \0"
        CompareStrncatEval(b_5, b_12, 4, "b_5, b_12");
        // Goal: "xxxx\0  \0\0aabb  \0"
        CompareStrncatEval(b_6, b_14, 2, "b_5, b_14");

        delete[] buff;
}
