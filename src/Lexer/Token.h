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

#ifndef APE_LANG_COMPILER_TOKEN_H
#define APE_LANG_COMPILER_TOKEN_H

#include <string>

using namespace std;

/**
 * Token - class designed to store lexemes from Lexer.
 * @author Danil Andreev
 */
class Token {
public:
    /**
     * TYPE - lexeme types enumeration.
     */
    enum TYPE {
        NUMBER,
        IDENTIFIER,
        KEYWORD,
        SYMBOL,
        LINEBREAK,
        EOFILE,
        STRING,
        UNSUPPORTED,
    };
protected:
    /**
     * type - lexeme type.
     */
    TYPE type;
    /**
     * payload - lexeme payload. Got from input text.
     */
    string payload;
public:
    Token(const TYPE type, const string payload);

    explicit Token(const TYPE type);

    Token(const Token &origin);

    /**
     * getType - getter for token type.
     * @author Danil Andreev
     */
    TYPE getType();

    /**
     * getType - getter for token payload.
     * @author Danil Andreev
     */
    string getPayload();
};

#endif //APE_LANG_COMPILER_TOKEN_H