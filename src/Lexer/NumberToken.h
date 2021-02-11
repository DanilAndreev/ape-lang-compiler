/*
MIT License

Copyright (c) 2020 Danil Maksimovich Andrieiev

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

#ifndef APE_LANG_COMPILER_NUMBERTOKEN_H
#define APE_LANG_COMPILER_NUMBERTOKEN_H

#include <regex>
#include "Token.h"

using namespace std;

/**
 * NumberToken - class designed to store numeric values from Lexer.
 * @author Danil Andreev
 */
class NumberToken: public Token {
protected:
    /**
     * NumberRegExp - RegExp for number correctness check.
     */
    static regex NumberRegExp;
    /**
     * value - stored numeric value.
     */
    long double value;
public:
    NumberToken(const string payload);
    /**
     * isInteger - method designed to check if the stored number is integer.
     * @return true if number is integer and false if not.
     */
    bool isInteger();
    /**
     * getDouble - getter for value.
     * @return value in float format.
     * @author Danil Andreev
     */
    long double getDouble();
    /**
     * getDouble - getter for value.
     * @return value in integer format.
     * @author Danil Andreev
     */
    long long getLong();
};


#endif //APE_LANG_COMPILER_NUMBERTOKEN_H
