#include "internal/_test_string.h"
#include <string.h>
#include <errno.h>

#include "internal/compare_buffer_functions.h"
#include "gtest/gtest.h"

/* The three following errno values are the only ones required by the C
 * standard; we'll likely add support for more when we begin implementing
 * extensions. 
 *  - EDOM
 *  - ERANGE
 *  - EILSEQ
 */

TEST(strerror, ErrEDOM) {
    EXPECT_STREQ(strerror(EDOM), _strerror(EDOM));
}

TEST(strerror, ErrERANGE) {
    EXPECT_STREQ(strerror(ERANGE), _strerror(ERANGE));
}

TEST(strerror, ErrEILSEQ) {
    EXPECT_STREQ(strerror(EILSEQ), _strerror(EILSEQ));
}

TEST(strerror, Basic) {
    EXPECT_STREQ(strerror(0), _strerror(0));
}
