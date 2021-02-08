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

set<wchar_t> Lexer::Symbols = set<wchar_t>{
        L'+',
        L'-',
        L'=',
        L'*',
        L'/',
        L'(',
        L')',
        L'"',
        L'\'',
        L';',
        L'!',
        L'<',
        L'>',
        L'{',
        L'}',
        L'[',
        L']',
};

set<wstring> Lexer::Keywords = set<wstring>{
        L"if",
        L"else",
        L"for",
        L"while",
        L"goto",

        L"switch",
        L"case",

        L"true",
        L"false"
};

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
    } else if (this->Symbols.find(character) != this->Symbols.end()) {
        return this->readSymbol();
    } else if (iswalpha(character)) {
        return this->readIdentifier();
    } else {
        throw exception();
    }
}

//NumberToken Lexer::readNumber() {
//    const wregex rx(L"[0-9\\-\\+\\e\\.]");
//
//    bool hasDot = false;
//    bool hasE = false;
//    bool hasESign = false;
//    wstring buffer = L"";
//
//    wchar_t previous = this->stream->get();
//    buffer += previous;
//    if (this->stream->eof()) return NumberToken(buffer);
//    wchar_t current = previous;
//
//    while (!this->stream->eof() && regex_search(&current, rx)) {
//        current = this->stream->get();
//        if (current == L'.' && !hasDot && !hasE) {
//            hasDot = true;
//        } else if (current == L'e' && !hasE) {
//            hasE = true;
//        } else if ((current == L'+' || current == L'-') && previous == L'e' && !hasESign && hasE) {
//            hasESign = true;
//        } else if (iswdigit(current)) {
//        } else {
//            if (previous == L'+' || previous == L'-') {
//                this->stream->unget();
//                buffer.erase(buffer.length() - 1, 1);
//                wcout << "erased" << buffer << endl;
//            }
//            break;
//        }
//        buffer += current;
//        wcout << buffer << endl;
//        previous = current;
//    }
//
//    this->stream->unget();
//    buffer.erase(buffer.length() - 1, 1);
//    wcout << "erased" << buffer << endl;
//
//    return NumberToken(buffer);
//}

//NumberToken Lexer::readNumber() {
//    wstring buffer = L"";
//    wchar_t character = this->stream->get();
//
//    while (!this->stream->eof() && (iswdigit(character) || character == L'.' || character == L'-' || character == L'e')) {
//        buffer += character;
//        this->stream->get(character);
//    }
//
//    if (!this->stream->eof()) this->stream->unget();
//
//    return NumberToken(buffer);
//}

Token Lexer::readIdentifier() {
    wstring buffer = L"";
    wchar_t character = this->stream->get();

    while (!this->stream->eof() && iswalnum(character)) {
        buffer += character;
        this->stream->get(character);
    }
    if (!this->stream->eof()) this->stream->unget();

    if (this->Keywords.find(buffer) != this->Keywords.end())
        return Token(Token::TYPE::KEYWORD, buffer);

    return Token(Token::TYPE::IDENTIFIER, buffer);
}

Token Lexer::readSymbol() {
    wstring buffer = L"";

    wchar_t character = this->stream->get();
    buffer += character;

    if (character == L'\n')
        return Token(Token::TYPE::LINEBREAK, buffer);

    return Token(Token::TYPE::SYMBOL, buffer);
}

bool Lexer::isCharacterSkippable(const wchar_t character) {
    return regex_search(&character, this->SkippableCharacters);
}
