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

#include <cmath>
#include "NumberToken.h"
#include <iostream>

using namespace std;

regex NumberToken::NumberRegExp = regex("[-+]?[0-9]*\\.?[0-9]+([eE][-+]?[0-9]+)?");

NumberToken::NumberToken(const string payload, const int line, const int column)
        : Token(Token::TYPE::NUMBER, payload, line, column) {
    if (!regex_match(payload, this->NumberRegExp)) {
        throw exception();
    }

    try {
        this->value = stold(payload);
    } catch (exception e) {
        throw exception();
    }
    this->classname = "NumberToken";
}

NumberToken::NumberToken(const NumberToken &reference) : Token(reference) {
    this->value = reference.value;
}

long double NumberToken::getDouble() const {
    return this->value;
}

bool NumberToken::isInteger() const {
    return trunc(this->value) == this->value && !this->payload.find('.');
}

long long NumberToken::getLong() const {
    if (!this->isInteger())
        throw exception(); // TODO: add normal exceptions system;
    return static_cast<long long>(this->value);
}

shared_ptr<Token> NumberToken::clone() const {
    return make_shared<NumberToken>(*this);
}
