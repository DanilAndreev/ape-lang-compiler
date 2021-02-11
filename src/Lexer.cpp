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
#include <locale>

using namespace std;

regex Lexer::SkippableCharacters = regex("[ \\t]+");

vector<string> Lexer::Symbols = vector<string>{
        "+",
        "-",
        "*",
        "/",
        "^",

        "=",
        "==",
        "!=",
        ">=",
        "<=",
        "<",
        ">",
        "!",

        "<<",
        ">>",

        "++",
        "--",

        "(",
        ")",
        "{",
        "}",
        "[",
        "]",

        ";",

        "\"",
        "'",
        "\n",
};

set<string> Lexer::Keywords = set<string>{
        "if",
        "else",
        "for",
        "while",
        "goto",

        "switch",
        "case",

        "true",
        "false"
};

Lexer::Lexer(istream *const stream) {
    this->stream = stream;

    this->symbols = new set<string, bool (*)(const string &, const string &)>(
            [](const string &lower, const string &higher) {
                if (lower.length() == higher.length()) return lower < higher;
                return lower.length() > higher.length();
            });
    this->symbols->insert(this->Symbols.begin(), this->Symbols.end());

    this->symbolsStartCharacters = new set<char>();
    for (const string &item : *this->symbols) {
        this->symbolsStartCharacters->insert(item.front());
    }
}

Lexer::~Lexer() {
    delete this->symbols;
    delete this->symbolsStartCharacters;
}

Token Lexer::nextToken() {
    char character;

    if (stream->eof()) return Token(Token::TYPE::EOFILE);

    // Skipping skippable characters.
    do {
        this->stream->get(character);
    } while (!stream->eof() && this->isCharacterSkippable(character));

    if (stream->eof()) return Token(Token::TYPE::EOFILE);
    this->stream->unget();

    // Determining token type
    if (isdigit(character)) {
        return this->readNumber();
    } else if (this->symbolsStartCharacters->find(character) != this->symbolsStartCharacters->end()) {
        return this->readSymbol();
    } else if (isalpha(character)) {
        return this->readIdentifier();
    } else {
        throw exception();
    }
}

NumberToken Lexer::readNumber() {
    bool hasDot = false;
    bool hasE = false;

    string buffer = "";
    char character = this->stream->get();

    while (!this->stream->eof()) {
        bool error = false;
        // Checking number structure
        if (character == L'.' && !hasDot && !hasE) {
            hasDot = true;
        } else if (character == L'e' && !hasE) {
            hasE = true;
        } else if ((character == L'+' || character == L'-') && buffer.back() == L'e') {
        } else if (isdigit(character)) {
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
                if (!isdigit(buffer.back())) {
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
    string buffer = "";
    char character = this->stream->get();

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
    string buffer;
    size_t length = 0;
    for (const string &item : *this->symbols) {
        if (length != item.length()) {
            long shift = (long) (buffer.length() * sizeof(char)) * -1;
            this->stream->seekg(shift, ios::cur);
            length = item.length();
            buffer = this->getFromStream(length);
        }
        if (item == buffer) {
            if (buffer == "\n")
                return Token(Token::TYPE::LINEBREAK, buffer);
            return Token(Token::TYPE::SYMBOL, buffer);
        }
    }
    for (size_t i = 0; i < length; i++)
        this->stream->unget();

    wchar_t character = this->stream->get();
    return Token(Token::TYPE::UNSUPPORTED, "" + character);
}

bool Lexer::isCharacterSkippable(const char character) {
    return iscntrl(character) || regex_search(&character, this->SkippableCharacters);
}

string Lexer::getFromStream(size_t length) {
    string buffer;
    for (size_t i = 0; i < length; i++) {
        if (this->stream->peek() == EOF)
            break;
        buffer += this->stream->get();
    }
    return buffer;
}
