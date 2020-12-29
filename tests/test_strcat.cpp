#include "aolc/_test_string.h"
#include <string.h>

#include "aolc/compare_buffer_functions.h"
#include "gtest/gtest.h"

void CompareStrcatEval(const char* s1, const char* s2, const char* comment) {
    SCOPED_TRACE(comment);
    CompareBufferFunctions<char*, char*, const char*>(
            _strcat, strcat, s1, s2,
            EqualityMode::kBufferRelativeEquality);
}

TEST(strcat, Basic)
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
        CompareStrcatEval(b_0, b_0, "b_0, b_0");
        // Goal: "xxxx  \0\0\0aabb  \0"
        CompareStrcatEval(b_0, b_10, "b_0, b_14");
        // Goal: "xxxx\0\0\0\0\0aabb  \0"
        CompareStrcatEval(b_0, b_5,  "b_0, b_5");
        // Goal: "xxxxbb  \0\0\0aabb  \0"
        CompareStrcatEval(b_0, b_12, "b_0, b_12");
        // Goal: "xxxxbb  \0\0\0aabb  \0"
        CompareStrcatEval(b_5, b_12, "b_5, b_12");
        // Goal: "xxxx\0  \0\0aabb  \0"
        CompareStrcatEval(b_6, b_14, "b_5, b_14");

        delete[] buff;
}
