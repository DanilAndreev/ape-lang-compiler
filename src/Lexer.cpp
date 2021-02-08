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

#include "Lexer.h"
#include <iostream>

using namespace std;

wregex Lexer::SkippableCharacters = wregex(L"[ \\t]+");
wregex Lexer::Symbols = wregex(L"[\\+\\-\\=\\/\\*\\(\\)\\\"\\'\\;]");


Lexer::Lexer(wistream *const stream) {
    this->stream = stream;
}

Lexer::~Lexer() {
    //delete this->stream;
}

Token Lexer::nextToken() {
    wchar_t character;

    if (stream->eof()) return Token(Token::TYPE::EOFILE);

    // Skipping skippable characters.
    do {
        this->stream->get(character);
    } while (!stream->eof() && this->isCharacterSkippable(character));

    if (stream->eof()) return Token(Token::TYPE::EOFILE);
    this->stream->unget();

    // Determining token type
    if (iswdigit(character)) {
        return this->readNumber();
    } else if (regex_search(&character, this->Symbols)) {
        return this->readSymbol();
    } else if (iswalpha(character)) {
        return this->readIdentifier();
    } else {
        throw exception();
    }
}

Token Lexer::readNumber() {
    wstring buffer = L"";
    wchar_t character = this->stream->get();

    while (!this->stream->eof() && iswdigit(character)) {
        buffer += character;
        this->stream->get(character);
    }

    return Token(Token::TYPE::NUMBER, buffer);
}

Token Lexer::readIdentifier() {
    wstring buffer = L"";
    wchar_t character = this->stream->get();

    while (!this->stream->eof() && iswalnum(character)) {
        buffer += character;
        this->stream->get(character);
    }

    return Token(Token::TYPE::IDENTIFIER, buffer);
}

Token Lexer::readSymbol() {
    wstring buffer = L"";

    wchar_t character = this->stream->get();
    buffer += character;

    return Token(Token::TYPE::SYMBOL, buffer);
}

bool Lexer::isCharacterSkippable(const wchar_t character) {
    return regex_search(&character, this->SkippableCharacters);
}
