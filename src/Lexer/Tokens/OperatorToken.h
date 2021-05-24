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

#ifndef APE_LANG_COMPILER_OPERATORTOKEN_H
#define APE_LANG_COMPILER_OPERATORTOKEN_H


#include "Token.h"

/**
 * OPERATORS - enumeration for operator types.
 * @author Danil Andreev
 */
enum OPERATORS {
    /// <
    LESS,
    /// <=
    LESS_EQUAL,
    /// >
    GREATER,
    /// >=
    GREATER_EQUAL,
    /// ==
    EQUAL,
    /// !=
    NOTEQUAL,
    /// =
    ASSIGN,
    /// (
    ROUND_BRACE_OPEN,
    /// )
    ROUND_BRACE_CLOSE,
    /// [
    SQUARE_BRACE_OPEN,
    /// ]
    SQUARE_BRACE_CLOSE,
    /// {
    CURLY_BRACE_OPEN,
    /// }
    CURLY_BRACE_CLOSE,
    /// +
    ADD,
    /// -
    SUBTRACT,
    /// *
    MULTIPLY,
    /// /
    DIVIDE,
    /// !
    NOT,
    /// ++
    INCREMENT,
    /// --
    DECREMENT,
    /// .
    DOT,
    /// ,
    COMA,
    /// ;
    SEMICOLON,
    /// \n
    LINEBREAK,
    /// "
    DOUBLE_QUOTES,
    /// '
    SINGLE_QUOTES,
    /// ^
    POWER,
    /// &&
    AND,
    /// ||
    OR,
};

/**
 * OperatorToken - class for storing operator token.
 * Contains enum operator type.
 * @author Danil Andreev
 */
class OperatorToken : public Token {
protected:
    /**
     * operatorType - enum operator type.
     */
    OPERATORS operatorType;
public:
    explicit OperatorToken(OPERATORS type, std::string payload = "", int line = 0, int column = 0);

    OperatorToken(const OperatorToken &reference);

    virtual std::shared_ptr<Token> clone() const;

public:
    /**
     * getOperatorType - getter for operator type.
     * @author Danil Andreev
     */
    OPERATORS getOperatorType() const;
};


#endif //APE_LANG_COMPILER_OPERATORTOKEN_H
