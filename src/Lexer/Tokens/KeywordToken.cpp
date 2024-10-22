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

#include "KeywordToken.h"

using namespace std;

KeywordToken::KeywordToken(KEYWORDS type, string payload, const int line, const int column)
        : Token(Token::KEYWORD, payload, line, column) {
    this->keywordType = type;
    this->classname = "KeywordToken";
}

KEYWORDS KeywordToken::getKeywordType() const {
    return this->keywordType;
}

KeywordToken::KeywordToken(const KeywordToken &reference) : Token(reference) {
    this->keywordType = reference.keywordType;
}

shared_ptr<Token> KeywordToken::clone() const {
    return make_shared<KeywordToken>(*this);
}
