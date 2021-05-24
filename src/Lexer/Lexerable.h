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

#ifndef APE_LANG_COMPILER_LEXERABLE_H
#define APE_LANG_COMPILER_LEXERABLE_H

#include <memory>
#include "Tokens/Token.h"

class Lexerable {
public:
    /**
     * nextToken - method, designed to get next token from input stream.
     * Also sets got token to lexer current token.
     * @return next token.
     * @author Danil Andreev
     */
    virtual std::shared_ptr <Token> nextToken() = 0;

    /**
     * getCurrentToken - getter for Lexer current token.
     * @author Danil Andreev
     */
    virtual std::shared_ptr <Token> getCurrentToken() const = 0;

    /**
     * isEof - returns true if eof flag has been set.
     * Else returns false.
     */
    virtual bool isEof() const = 0;
};

#endif //APE_LANG_COMPILER_LEXERABLE_H
