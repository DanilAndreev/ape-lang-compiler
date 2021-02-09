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

vector<wstring> Lexer::Symbols = vector<wstring>{
        L"+",
        L"-",
        L"*",
        L"/",
        L"^",

        L"=",
        L"==",
        L"!=",
        L">=",
        L"<=",
        L"<",
        L">",
        L"!",

        L"<<",
        L">>",

        L"++",
        L"--",

        L"(",
        L")",
        L"{",
        L"}",
        L"[",
        L"]",

        L";",

        L"\"",
        L"'",
        L"\n",
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

    this->symbols = new set<wstring, bool (*)(const wstring &, const wstring &)>(
            [](const wstring &lower, const wstring &higher) {
                if (lower.length() == higher.length()) return lower < higher;
                return lower.length() > higher.length();
            });
    this->symbols->insert(this->Symbols.begin(), this->Symbols.end());

    this->symbolsStartCharacters = new set<wchar_t>();
    for (const wstring &item : *this->symbols) {
        this->symbolsStartCharacters->insert(item.front());
    }
}

Lexer::~Lexer() {
    delete this->symbols;
    delete this->symbolsStartCharacters;
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
    } else if (this->symbolsStartCharacters->find(character) != this->symbolsStartCharacters->end()) {
        return this->readSymbol();
    } else if (iswalpha(character)) {
        return this->readIdentifier();
    } else {
        throw exception();
    }
}

NumberToken Lexer::readNumber() {
    bool hasDot = false;
    bool hasE = false;

    wstring buffer = L"";
    wchar_t character = this->stream->get();

    while (!this->stream->eof()) {
        bool error = false;
        // Checking number structure
        if (character == L'.' && !hasDot && !hasE) {
            hasDot = true;
        } else if (character == L'e' && !hasE) {
            hasE = true;
        } else if ((character == L'+' || character == L'-') && buffer.back() == L'e') {
        } else if (iswdigit(character)) {
        } else {
            error = true;
        }

        if (!error) {
            buffer += character;
            this->stream->get(character);
        } else {
            this->stream->unget();
            // Rolling back to last correct number
            bool cleared = false;
            while (!cleared) {
                if (!iswdigit(buffer.back())) {
                    buffer.erase(buffer.length() - 1, 1);
                    this->stream->unget();
                } else {
                    cleared = true;
                }
            }
            break;
        }
    }

    return NumberToken(buffer);
}

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
    wstring buffer;
    size_t length = 0;
    for (const wstring &item : *this->symbols) {
        if (length != item.length()) {
            for (size_t i = 0; i < buffer.length(); i++) {
                wcout << "ungetting" << endl;
                this->stream->unget();
            }
//            this->stream->seekg(-length, ios::cur);
            length = item.length();
            buffer = this->getFromStream(length);
        }
        wcout << "comparing: '" << buffer << "' with '" << item << "'" << endl;
        if (item == buffer) {
            if (buffer == L"\n")
                return Token(Token::TYPE::LINEBREAK, buffer);
            return Token(Token::TYPE::SYMBOL, buffer);
        }
    }
    for (size_t i = 0; i < length; i++)
        this->stream->unget();

    wchar_t character = this->stream->get();
    wcout << "is eof?: " << this->stream->eof() << " unsupported: '" << (char)character << "'" << endl;
    return Token(Token::TYPE::UNSUPPORTED, L"" + character);
}

//Token Lexer::readSymbol() {
//    wstring buffer = L"";
//
//    wchar_t character = this->stream->get();
//    buffer += character;
//
//    if (character == L'\n')
//        return Token(Token::TYPE::LINEBREAK, buffer);
//
//    return Token(Token::TYPE::SYMBOL, buffer);
//}

bool Lexer::isCharacterSkippable(const wchar_t character) {
    return regex_search(&character, this->SkippableCharacters);
}

wstring Lexer::getFromStream(size_t length) {
    wstring buffer;
    for (size_t i = 0; i < length; i++) {
        if (this->stream->eof()) break;
        wchar_t character = this->stream->get();
//        wcout << "read character[iswascii=" << iswascii(character) << "]: '" << character << "'" << endl;
        if (iswascii(character)) {
            buffer += character;
        } else {
            wcout << "ungetting" << endl;
            this->stream->unget();
        }
//        wcout << "got from stream[" << buffer.length() << "]: '" << buffer << "'" << endl;
    }
    return buffer;


//    wchar_t array[length + 1];
//    wcout << "got array: '" << array << "'" << endl;
//    this->stream->get(array, length);
//    array[length + 1] = L'\0';
//    return wstring(array);
}
