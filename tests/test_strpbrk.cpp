#include "aolc/_test_string.h"
#include <string.h>

#include "aolc/compare_buffer_functions.h"
#include "gtest/gtest.h"

#include <iostream>

void CompareStrpbrkEval(const char* s1,
                        const char* s2,
                        const char* comment) {
    using std::placeholders::_1;
    using std::placeholders::_2;

    SCOPED_TRACE(comment);
    CompareBufferFunctions<char*, char*, const char*>(
        _strpbrk, static_cast<char* (*)(char*, const char*)>(strpbrk), s1, s2,
        EqualityMode::kBufferRelativeEquality);
}


TEST(strpbrk, OneTokenSucc) {
    CompareStrpbrkEval("r", "r", "#1");
    CompareStrpbrkEval("rrrr", "r", "#2");
    CompareStrpbrkEval("oooor", "r", "#3");
    CompareStrpbrkEval("zzzz_zz", "_", "#4");
    CompareStrpbrkEval("zzzz_zz", "_", "#5");
}

TEST(strpbrk, OneTokenFail) {
    CompareStrpbrkEval("x", "X", "#1");
    CompareStrpbrkEval("x", "&", "#1");
    CompareStrpbrkEval("xabcabc", "_", "#2");
    CompareStrpbrkEval("xabcabc", "$", "#2");
    CompareStrpbrkEval("xabcabc\n$", "$", "#3");
}

TEST(strpbrk, CharAlignment) {
    // Note that the ASCII value of '_' is 0xF2 == 0b11110010
    char base[] =   {(char)0b10000000, (char)0b11110010, (char)0b00000001, '\n'};
    char off_1r[] = {(char)0b10000000, (char)0b01111001, (char)0b00000001, '\n'};
    char off_2r[] = {(char)0b10000000, (char)0b00111100, (char)0b10000001, '\n'};
    char off_3r[] = {(char)0b10000000, (char)0b00011110, (char)0b01000001, '\n'};
    char off_4r[] = {(char)0b10000000, (char)0b00001111, (char)0b00100001, '\n'};
    char off_1l[] = {(char)0b10000001, (char)0b11100100, (char)0b00000001, '\n'};
    char off_2l[] = {(char)0b10000011, (char)0b11001000, (char)0b00000001, '\n'};
    char off_3l[] = {(char)0b10000111, (char)0b10010000, (char)0b00000001, '\n'};
    char off_4l[] = {(char)0b10001111, (char)0b00100000, (char)0b00000001, '\n'};
    
    CompareStrpbrkEval(base,   "_", "base");
    CompareStrpbrkEval(off_1r, "_", "off_1r");
    CompareStrpbrkEval(off_2r, "_", "off_2r");
    CompareStrpbrkEval(off_3r, "_", "off_3r");
    CompareStrpbrkEval(off_4r, "_", "off_4r");
    CompareStrpbrkEval(off_1l, "_", "off_1l");
    CompareStrpbrkEval(off_2l, "_", "off_2l");
    CompareStrpbrkEval(off_3l, "_", "off_3l");
    CompareStrpbrkEval(off_4l, "_", "off_4l");
}

TEST(strpbrk, Basic) {
    char str[] = {'x', 'x', 'x', 'X', 'y', 'X', 'X', '\0'};
    char x[]  = "x";
    char xX[] = "xX";
    char X[]  = "X";
    char y[]  = "y";
    char _y[] = "asdasdasdasdasdasdaasdgqweqweasdqweqqe_y";

    CompareStrpbrkEval(str, x, "str, x");
    CompareStrpbrkEval(str, xX, "str, xX");
    CompareStrpbrkEval(str, X, "str, X");
    CompareStrpbrkEval(str, y, "str, y");
    CompareStrpbrkEval(str, str, "str, str");
    CompareStrpbrkEval(str + 1, str, "str + 1, str");
    CompareStrpbrkEval(_y + 10, _y, "_y, _y");
    CompareStrpbrkEval(str, _y, "str, _y");
}

TEST(strpbrk, LongPrintable) {
    // Fill buffer with pseudo-randomly (still deterministic) selected
    // printable characters ([' ', '1']).
    auto PopulateString = [](char* str_buffer, size_t len, long int seed) {
        const long unsigned int kMinChar = ' ';
        const long unsigned int kMaxChar = '~';
        const long unsigned int kCharRange = kMaxChar - kMinChar;

        long unsigned int c = (seed - kMinChar) % kCharRange;
        for (size_t i = 0; i < len; i++) {
            str_buffer[i] = c + kMinChar;
            c = ((((10067*(c + kMinChar)) ^ 2147483647) & 0x00FFFF00) >> 8) % kCharRange;
        }
    };

    char* buffer = new char[4095];
    memset(buffer, '\0', 4097);

    PopulateString(buffer, 512, 'j');
    CompareStrpbrkEval(buffer, buffer + 3/4*(512), "Len 512");

    PopulateString(buffer, 1024, ')');
    CompareStrpbrkEval(buffer, buffer + 3/4*(1024), "Len 1024");

    PopulateString(buffer, 2048, ' ');
    CompareStrpbrkEval(buffer, buffer + 3/4*2048, "Len 2048");

    PopulateString(buffer, 4094, 'A');
    CompareStrpbrkEval(buffer, buffer + 3/4*4094, "Len 4094");
}
