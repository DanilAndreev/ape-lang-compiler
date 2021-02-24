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

vector<pair<string, OPERATORS>> Lexer::Symbols = vector<pair<string, OPERATORS>>{
        pair<string, OPERATORS>("+", OPERATORS::ADD),
        pair<string, OPERATORS>("-", OPERATORS::SUBTRACT),
        pair<string, OPERATORS>("*", OPERATORS::MULTIPLY),
        pair<string, OPERATORS>("/", OPERATORS::DIVIDE),
        pair<string, OPERATORS>("^", OPERATORS::XOR),
        pair<string, OPERATORS>("=", OPERATORS::ASSIGN),
        pair<string, OPERATORS>("==", OPERATORS::EQUAL),
        pair<string, OPERATORS>("!=", OPERATORS::NOTEQUAL),
        pair<string, OPERATORS>(">=", OPERATORS::GREATER_EQUAL),
        pair<string, OPERATORS>("<=", OPERATORS::LESS_EQUAL),
        pair<string, OPERATORS>("<", OPERATORS::LESS),
        pair<string, OPERATORS>(">", OPERATORS::GREATER),
        pair<string, OPERATORS>("!", OPERATORS::NOT),
        pair<string, OPERATORS>("++", OPERATORS::INCREMENT),
        pair<string, OPERATORS>("(", OPERATORS::ROUND_BRACE_OPEN),
        pair<string, OPERATORS>(")", OPERATORS::ROUND_BRACE_CLOSE),
        pair<string, OPERATORS>("{", OPERATORS::CURLY_BRACE_OPEN),
        pair<string, OPERATORS>("}", OPERATORS::CURLY_BRACE_CLOSE),
        pair<string, OPERATORS>("[", OPERATORS::SQUARE_BRACE_OPEN),
        pair<string, OPERATORS>("]", OPERATORS::SQUARE_BRACE_CLOSE),
        pair<string, OPERATORS>(";", OPERATORS::SEMICOLON),
        pair<string, OPERATORS>(",", OPERATORS::COMA),
        pair<string, OPERATORS>(".", OPERATORS::DOT),
        pair<string, OPERATORS>("\"", OPERATORS::DOUBLE_QUOTES),
        pair<string, OPERATORS>("'", OPERATORS::SINGLE_QUOTES),
        pair<string, OPERATORS>("\n", OPERATORS::LINEBREAK),
//        "<<",
//        ">>",
};

set<pair<string, KEYWORDS>> Lexer::Keywords = set<pair<string, KEYWORDS>>{
        pair<string, KEYWORDS>("if", KEYWORDS::IF),
        pair<string, KEYWORDS>("else", KEYWORDS::ELSE),
        pair<string, KEYWORDS>("for", KEYWORDS::FOR),
        pair<string, KEYWORDS>("while", KEYWORDS::WHILE),
        pair<string, KEYWORDS>("do", KEYWORDS::DO),
        pair<string, KEYWORDS>("goto", KEYWORDS::GOTO),

        pair<string, KEYWORDS>("const", KEYWORDS::CONST),

        pair<string, KEYWORDS>("switch", KEYWORDS::SWITCH),
        pair<string, KEYWORDS>("case", KEYWORDS::CASE),

        pair<string, KEYWORDS>("true", KEYWORDS::TRUE),
        pair<string, KEYWORDS>("false", KEYWORDS::FALSE),

        pair<string, KEYWORDS>("int", KEYWORDS::INT),
        pair<string, KEYWORDS>("float", KEYWORDS::FLOAT),
        pair<string, KEYWORDS>("string", KEYWORDS::STRING),
        pair<string, KEYWORDS>("boolean", KEYWORDS::BOOLEAN)
};

Lexer::Lexer(istream *const stream) {
    this->stream = stream;
    this->currentToken = nullptr;
    this->eof = false;

    this->symbols = new set<pair<string, OPERATORS>, bool (*)(const pair<string, OPERATORS> &,
                                                              const pair<string, OPERATORS> &)>(
            [](const pair<string, OPERATORS> &lower, const pair<string, OPERATORS> &higher) {
                if (lower.first.length() == higher.first.length()) return lower < higher;
                return lower.first.length() > higher.first.length();
            });
    this->symbols->insert(this->Symbols.begin(), this->Symbols.end());

    this->symbolsStartCharacters = new set<char>();
    for (const pair<string, OPERATORS> &item : *this->symbols) {
        this->symbolsStartCharacters->insert(item.first.front());
    }
}

Lexer::Lexer(const Lexer &reference) : Lexer(reference.stream) {
    this->currentToken = reference.currentToken;
    this->eof = reference.eof;
}

Lexer::~Lexer() {
    delete this->symbols;
    delete this->symbolsStartCharacters;
    this->currentToken.reset();
}

Token *Lexer::getNextToken() {
    char character;

    if (stream->eof()) return new Token(Token::TYPE::EOFILE);

    // Skipping skippable characters.
    do {
        this->stream->get(character);
    } while (!stream->eof() && this->isCharacterSkippable(character));

    if (stream->eof()) return new Token(Token::TYPE::EOFILE);
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


shared_ptr<Token> Lexer::nextToken() {
    Token *result = this->getNextToken();
//    this->currentToken.reset();
    this->currentToken = result->clone();
    this->eof = false;
    if (this->currentToken->getType() == Token::TYPE::EOFILE)
        this->eof = true;
    cout << "Lexer: got token: " << result->getType() << " | payload: \"" << result->getPayload() << "\";" << endl;
    return shared_ptr<Token>(result);
}

NumberToken *Lexer::readNumber() {
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

    return new NumberToken(buffer);
}

Token *Lexer::readIdentifier() {
    string buffer = "";
    char character = this->stream->get();

    while (!this->stream->eof() && iswalnum(character)) {
        buffer += character;
        this->stream->get(character);
    }
    if (!this->stream->eof()) this->stream->unget();

    for (const pair<string, KEYWORDS> item : this->Keywords) {
        if (item.first == buffer)
            return new KeywordToken(item.second, buffer);
    }

//    if (this->Keywords.find(buffer) != this->Keywords.end())
//        return Token(Token::TYPE::KEYWORD, buffer);

    return new Token(Token::TYPE::IDENTIFIER, buffer);
}

Token *Lexer::readSymbol() {
    string buffer;
    size_t length = 0;
    for (const pair<string, OPERATORS> &item : *this->symbols) {
        const string item_str = item.first;
        if (length != item_str.length()) {
            long shift = (long) (buffer.length() * sizeof(char)) * -1;
            this->stream->seekg(shift, ios::cur);
            length = item_str.length();
            buffer = this->getFromStream(length);
        }
        if (item_str == buffer) {
            if (buffer == "\"" || buffer == "'") {
                this->stream->unget();
                return this->readString();
            }
            if (buffer == "\n")
                return new Token(Token::TYPE::LINEBREAK, buffer);
            return new OperatorToken(item.second, buffer);
        }
    }
    for (size_t i = 0; i < length; i++)
        this->stream->unget();

    wchar_t character = this->stream->get();
    return new Token(Token::TYPE::UNSUPPORTED, "" + character);
}

Token *Lexer::readString() {
    string buffer;
    const char brace = this->stream->get();
    char character;
    while (!this->stream->eof()) {
        this->stream->get(character);
        if (character == brace)
            break;

        if (character == '\n') {
            this->stream->unget();
            return new Token(Token::TYPE::STRING, buffer);
        }
        if (character == '\\') {
            char next = this->stream->get();
            if (next == EOF)
                throw new exception();

            switch (next) {
                case 'n':
                    buffer += '\n';
                    break;
                case 'r':
                    buffer += '\r';
                    break;
                case 't':
                    buffer += '\t';
                    break;
                default:
                    buffer += next;
            }
        } else {
            buffer += character;
        }
    }
    return new Token(Token::TYPE::STRING, buffer);
}

bool Lexer::isCharacterSkippable(const char character) {
    return static_cast<bool>(iscntrl(character)) || regex_search(&character, this->SkippableCharacters);
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

shared_ptr<Token> Lexer::getCurrentToken() const {
    return shared_ptr<Token>(this->currentToken);
}

bool Lexer::isEof() const {
    return this->eof;
}
