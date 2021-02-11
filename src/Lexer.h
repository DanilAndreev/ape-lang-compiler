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

#ifndef APE_LANG_COMPILER_LEXER_H
#define APE_LANG_COMPILER_LEXER_H

#include <istream>
#include <regex>
#include <set>
#include <vector>
#include <string>
#include "Token.h"
#include "NumberToken.h"

using namespace std;

class Lexer {
protected:
    static regex SkippableCharacters;
    static vector<string> Symbols;
    static set<string> Keywords;
    istream *stream;
    set<string, bool(*)(const string&, const string&)>* symbols;
    set<char>* symbolsStartCharacters;
public:
    Lexer(istream *const stream);

    ~Lexer();

    Token nextToken();

protected:
    NumberToken readNumber();

    Token readIdentifier();

    Token readSymbol();

protected:
    bool isCharacterSkippable(const char character);

    string getFromStream(size_t length);
};


#endif //APE_LANG_COMPILER_LEXER_H
