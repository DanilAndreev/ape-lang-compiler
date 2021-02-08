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

using namespace std;

wregex Lexer::SkippableCharacters = wregex(L"abc");

Lexer::Lexer(istream* const stream) {
    this->stream = stream;
}

Lexer::~Lexer() {
    //delete this->stream;
}

Token Lexer::nextToken() {
    wstring buffer;
    wchar_t character;

    if (stream->eof()) return Token(Token::TYPE::EOFILE);

    // Skipping skipable characters.
    do {
        character = this->stream->get();
    } while (!stream->eof() && this->isCharacterSkippable(character));

    if (stream->eof()) return Token(Token::TYPE::EOFILE);



    return Token(Token::TYPE::NUMBER, buffer);
}

Token Lexer::readNumber() {
    wstring buffer = L"";


    return Token(Token::TYPE::NUMBER, buffer);
}

Token Lexer::readIdentfier() {
    wstring buffer = L"";


    return Token(Token::TYPE::IDENTIFIER, buffer);
}

Token Lexer::readSymbol() {
    wstring buffer = L"";


    return Token(Token::TYPE::SYMBOL, buffer);
}

bool Lexer::isCharacterSkippable(const wchar_t character) {
    return regex_match(&character, this->SkippableCharacters);
}
