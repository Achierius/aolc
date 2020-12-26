#include "aolc/_test_string.h"
#include <string.h>
#include <stddef.h>

#include "aolc/compare_buffer_functions.h"
#include "gtest/gtest.h"

#include <functional>

void CompareStrcpyEval(const char* s1, const char* s2, const char* comment) {
    SCOPED_TRACE(comment);
    CompareBufferFunctions<char, char, const char>(_strcpy, strcpy, s1, s2);
}

TEST(strcpy, Basic)
{
    using std::placeholders::_1;
    using std::placeholders::_2;

    const char dest[] = {'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', '\0'};
    const char src_nonull[] = {'x', 'x', 'x', 'X', 'X', '\0'};
    const char src_yesnull[] = {'y', 'y', 'y', '\0'};
    const char goal[] = {'x', 'x', 'y', 'y', 'y', '\0', 'g', 'h', 'i', 'j', '\0'};



    CompareStrcpyEval(dest, src_nonull, "dest, src-nonull");
    CompareStrcpyEval(dest, src_yesnull, "dest, yesnull");
}
