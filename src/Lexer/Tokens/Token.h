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
#include <memory>

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
        EMPTY,
    };
protected:
    std::string classname;
    /// type - lexeme type.
    TYPE type;
    /// payload - lexeme payload. Got from input text.
    std::string payload;
    /// Token line in input file.
    int line;
    /// Token column in input file.
    int column;
public:
    Token(TYPE type, std::string payload = "", int line = -1, int column = -1);

    Token(const Token &reference);

    virtual std::shared_ptr<Token> clone() const;

public:
    /**
     * getType - getter for token type.
     * @author Danil Andreev
     */
    TYPE getType() const;

    /**
     * getType - getter for token payload.
     * @author Danil Andreev
     */
    virtual std::string getPayload() const;

    /**
     * getLine - getter for token line in input file.
     * @author Danil Andreev
     */
    int getLine() const;

    /**
     * getColumn - getter for token column in input file.
     * @author Danil Andreev
     */
    int getColumn() const;
};

#endif //APE_LANG_COMPILER_TOKEN_H
