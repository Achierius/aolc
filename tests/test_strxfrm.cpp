#include "aolc/_test_string.h"
#include <string.h>

#include "aolc/compare_buffer_functions.h"
#include "gtest/gtest.h"

void CompareStrxfrmEval(const char* s1,
                        const char* s2,
                        size_t n,
                        const char* comment) {
    using std::placeholders::_1;
    using std::placeholders::_2;

    auto true_fn = std::bind(strxfrm, _1, _2, n);
    auto test_fn = std::bind(_strxfrm, _1, _2, n);
    SCOPED_TRACE(comment);
    CompareBufferFunctions<size_t, char*, const char*>(
            test_fn, true_fn, s1, s2, strlen(s1), strlen(s2),
            EqualityMode::kStrictEquality);
}


/* TODO actually test locale-dependent behavior */

TEST(strxfrm, Basic) {
    char* buffer = new char[128];

    const char* hello = "hello";
    const char* xd = "xd";

    CompareStrxfrmEval(buffer, hello, 5, "hello, 5");
    CompareStrxfrmEval(buffer, hello, 3, "hello, 3");
    CompareStrxfrmEval(buffer, xd, 2, "xd, 3");
}
