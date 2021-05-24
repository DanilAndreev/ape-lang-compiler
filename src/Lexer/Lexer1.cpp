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

#include "Lexer1.h"
#include "../exceptions/ApeCompilerException.h"
#include "Tokens/NumberToken.h"

using namespace std;

regex Lexer1::SkippableCharacters = regex("[ \\t]+");

vector<pair<string, OPERATORS>> Lexer1::Symbols = vector<pair<string, OPERATORS>>{
        {"+",  OPERATORS::ADD},
        {"-",  OPERATORS::SUBTRACT},
        {"*",  OPERATORS::MULTIPLY},
        {"/",  OPERATORS::DIVIDE},
        {"^",  OPERATORS::POWER},
        {"=",  OPERATORS::ASSIGN},
        {"==", OPERATORS::EQUAL},
        {"!=", OPERATORS::NOTEQUAL},
        {">=", OPERATORS::GREATER_EQUAL},
        {"<=", OPERATORS::LESS_EQUAL},
        {"<",  OPERATORS::LESS},
        {">",  OPERATORS::GREATER},
        {"!",  OPERATORS::NOT},
        {"++", OPERATORS::INCREMENT},
        {"(",  OPERATORS::ROUND_BRACE_OPEN},
        {")",  OPERATORS::ROUND_BRACE_CLOSE},
        {"{",  OPERATORS::CURLY_BRACE_OPEN},
        {"}",  OPERATORS::CURLY_BRACE_CLOSE},
        {"[",  OPERATORS::SQUARE_BRACE_OPEN},
        {"]",  OPERATORS::SQUARE_BRACE_CLOSE},
        {";",  OPERATORS::SEMICOLON},
        {",",  OPERATORS::COMA},
        {".",  OPERATORS::DOT},
        {"&&", OPERATORS::AND},
        {"||", OPERATORS::OR},
        {"\"", OPERATORS::DOUBLE_QUOTES},
        {"'",  OPERATORS::SINGLE_QUOTES},
        {"\n", OPERATORS::LINEBREAK},
};

set<pair<string, KEYWORDS>> Lexer1::Keywords = set<pair<string, KEYWORDS>>{
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

        pair<string, KEYWORDS>("print", KEYWORDS::PRINT),
        pair<string, KEYWORDS>("read", KEYWORDS::READ),

        pair<string, KEYWORDS>("int", KEYWORDS::INT),
        pair<string, KEYWORDS>("float", KEYWORDS::FLOAT),
        pair<string, KEYWORDS>("string", KEYWORDS::STRING),
        pair<string, KEYWORDS>("boolean", KEYWORDS::BOOLEAN)
};

Lexer1::Lexer1(istream &input_stream) : stream(input_stream) {
    this->line = 0;
    this->column = 0;
    this->lineText = string();
    this->currentToken = nullptr;
    this->currentCharacter = '\0';

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

Lexer1::Lexer1(const Lexer1 &reference) : Lexer1(reference.stream) {
    this->line = reference.line;
    this->column = reference.column;
    this->lineText = reference.lineText;
    this->currentToken = reference.currentToken;
    this->currentCharacter = reference.currentCharacter;
}

Lexer1::~Lexer1() {
}

std::shared_ptr<Token> Lexer1::nextToken() {
    shared_ptr<Token> result = this->getNextToken();
    this->currentToken = result->clone();
    //TODO: check if custom EOF  is deprecated.
#ifndef NDEBUG
    cout << "Lexer: got token: " << result->getType() << " | payload: \"" << result->getPayload() << "\"; "
         << result->getLine() + 1 << ":" << result->getColumn() + 1 << endl;
#endif
    return result;
}

std::shared_ptr<Token> Lexer1::getCurrentToken() const {
    return this->currentToken;
}

bool Lexer1::isEof() const {
    return this->stream.eof();
}

char Lexer1::get() {
    char symbol = static_cast<char>(this->stream.get());
    if (symbol == '\n') {
        this->column = 0;
        this->line++;
        this->lineText.clear();
    } else {
        this->column++;
        this->lineText += symbol;
    }
    this->currentCharacter = symbol;
    return symbol;
}

void Lexer1::get(char &input) {
    input = this->get();
}

std::string Lexer1::get(size_t length) {
    if (!length) return "";
    string buffer;
    for (size_t i = 0; i < length && !this->stream.eof(); i++) {
        buffer += static_cast<char>(this->get());
    }
    return buffer;
}

std::string Lexer1::oversee(std::size_t length) {
    if (!length) return "";
    char current = this->currentCharacter;
    int currentLine = this->line;
    int currentColumn = this->column;
    string buffer = this->get(length);
    this->stream.seekg(-((long) buffer.length()), ios_base::cur);
    this->currentCharacter = current;
    this->column = currentColumn;
    this->line = currentLine;
    return buffer;
}

std::shared_ptr<Token> Lexer1::getNextToken() {
    char character;
    if (stream.eof()) return make_shared<Token>(Token::EOFILE);

    character = this->currentCharacter;
    while ((!stream.eof() && this->isCharacterSkipable(character)) || character == '\0') {
        this->get(character);
    }

    if (stream.eof()) return make_shared<Token>(Token::TYPE::EOFILE);

    // Determining token type
    if (isdigit(character)) {
        return this->readNumber();
    } else if (this->symbolsStartCharacters->find(character) != this->symbolsStartCharacters->end()) {
        return this->readSymbol();
    } else if (isalpha(character)) {
        return this->readIdentifier();
    } else {
        throw ApeCompilerException("Error");
    }
}

bool Lexer1::isCharacterSkipable(const char character) {
    return static_cast<bool>(iscntrl(character)) || regex_search(&character, this->SkippableCharacters);
}

std::shared_ptr<Token> Lexer1::readNumber() {
    const int tokenLine = this->line;
    const int tokenColumn = this->column - 1;

    bool hasDot = false;
    bool hasE = false;

    string buffer;
    char character = this->currentCharacter;

    while (!this->stream.eof()) {
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
            this->get(character);
        } else {
            break;
        }
    }

    return make_shared<NumberToken>(buffer, tokenLine, tokenColumn);
}

std::shared_ptr<Token> Lexer1::readIdentifier() {
    const int tokenLine = this->line;
    const int tokenColumn = this->column - 1;

    string buffer;
    char character = this->currentCharacter;

    while (!this->stream.eof() && isalnum(character)) {
        buffer += character;
        this->get(character);
    }

    for (const pair<string, KEYWORDS> item : this->Keywords) {
        if (item.first == buffer)
            return make_shared<KeywordToken>(item.second, buffer, tokenLine, tokenColumn);
    }

    return make_shared<Token>(Token::TYPE::IDENTIFIER, buffer, tokenLine, tokenColumn);

}

std::shared_ptr<Token> Lexer1::readSymbol() {
    const int tokenLine = this->line;
    const int tokenColumn = this->column - 1;

    const string start(1, this->currentCharacter);
    for (const pair<string, OPERATORS> &item : *this->symbols) {
        const string itemStr = item.first;

        const string buffer = start + this->oversee(itemStr.length() - 1);
        if (itemStr == buffer) {
            this->get(itemStr.length());
            if (itemStr == "\n")
                return make_shared<Token>(Token::TYPE::LINEBREAK, itemStr, tokenLine, tokenColumn);
            return make_shared<OperatorToken>(item.second, itemStr, tokenLine, tokenColumn);
        }
    }
    string character;
    character += this->get();
    return make_shared<Token>(Token::TYPE::UNSUPPORTED, character, tokenLine, tokenColumn);
}

std::shared_ptr<Token> Lexer1::readString() {
    const int tokenLine = this->line;
    const int tokenColumn = this->column - 1;

    string buffer;
    const char brace = this->get();
    char character;
    while (!this->stream.eof()) {
        this->get(character);
        if (character == brace)
            break;

        if (character == '\n') {
            return make_shared<Token>(Token::TYPE::STRING, buffer, tokenLine, tokenColumn);
        }
        if (character == '\\') {
            char next = this->get();
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
    return make_shared<Token>(Token::TYPE::STRING, buffer, tokenLine, tokenColumn);
}
