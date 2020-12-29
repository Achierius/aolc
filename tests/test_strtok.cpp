#include "aolc/_test_string.h"
#include <string.h>

#include "aolc/compare_buffer_functions.h"
#include "gtest/gtest.h"

void CompareStrtokEval(const char* s1, const char* s2, const char* comment) {
    SCOPED_TRACE(comment);
    CompareBufferFunctions<char*, char*, const char*>(
            _strtok, strtok, s1, s2,
            EqualityMode::kBufferRelativeEquality);
}

TEST(strtok, Basic)
{
    char* buff = new char[5];
    memcpy(buff, "x;x;\0", 5);
    CompareStrtokEval(buff, ";", ";");
    CompareStrtokEval(buff, "x", "x");
    CompareStrtokEval(buff, "@", "@");
}

TEST(strtok, Null)
{
    char* buff = new char[6];
    memcpy(buff, "x;x;\0\0\0\0", 6);
    CompareStrtokEval(buff, "\0", "buff, \\0 #1");
    buff = strtok(buff, "\0");
    CompareStrtokEval(buff, "\0", "buff, \\0 #2");
    buff = strtok(buff, "\0");
    CompareStrtokEval(buff, "\0", "buff, \\0 #3");
    buff = strtok(buff, "\0");
    CompareStrtokEval(buff, "\0", "buff, \\0 #4");
}

TEST(strtok, Degenerate)
{
    char* buff_a = new char[16];
    char* buff_b = new char[16];
    memcpy(buff_a, "@@@@@@@@@@@@@@@\0", 16);
    memcpy(buff_b, "@@@@@@@@@@@@@@@\0", 16);

    char* ptr_a = buff_a;
    CompareStrtokEval(ptr_a, "@", "ptr_a, @");
    ptr_a = strtok(ptr_a, "@");

    char* ptr_b = buff_b;
    CompareStrtokEval(ptr_b, ";", "ptr_b, ;");
    ptr_b = strtok(ptr_b, ";");
}
