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

#include "Token.h"

Token::Token(const Token::TYPE type, const string payload, const int line, const int column) {
    this->type = type;
    this->payload = payload;
    this->classname = "Token";
    this->line = line;
    this->column = column;
}

Token::Token(const Token &reference) {
    this->type = reference.type;
    this->payload = reference.payload;
    this->classname = reference.classname;
    this->line = reference.line;
    this->column = reference.column;
}

Token::TYPE Token::getType() const {
    return this->type;
}

string Token::getPayload() const {
    return this->payload;
}

shared_ptr<Token> Token::clone() const {
    return make_shared<Token>(*this);
}

int Token::getLine() const {
    return this->line;
}

int Token::getColumn() const {
    return this->column;
}
