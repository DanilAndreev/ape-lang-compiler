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

/**
 * Lexer - class, designed to create tokens from text.
 * Use Lexer::nextToken to get next token.
 * @note Input stream should will not be deleted on class destruction.
 * @author Danil Andreev
 */
class Lexer {
protected:
    /**
     * SkippableCharacters - RegExp for skipping characters.
     */
    static regex SkippableCharacters;
    /**
     * Symbols - an array of lang symbols/operators.
     * Will be added to set.
     */
    static vector<string> Symbols;
    /**
     * Keywords - set of lang keywords.
     */
    static set<string> Keywords;
    /**
     * stream - input data stream with program code.
     */
    istream *stream;
    /**
     * symbols - set of lang symbols and operators, ordered by length DESC.
     */
    set<string, bool (*)(const string &, const string &)> *symbols;
    /**
     * symbolsStartCharacters - a set of first characters from symbols set for lexer.
     */
    set<char> *symbolsStartCharacters;
public:
    explicit Lexer(istream *const stream);

    ~Lexer();

    /**
     * nextToken - method, designed to get next token from input stream.
     * @return next token.
     * @author Danil Andreev
     */
    Token nextToken();

protected:
    /**
     * readNumber - method, designed to read number from the stream.
     * @author Danil Andreev
     */
    NumberToken readNumber();

    /**
     * readNumber - method, designed to read identifier from the stream.
     * @author Danil Andreev
     */
    Token readIdentifier();

    /**
     * readNumber - method, designed to read symbol/operator from the stream.
     * @author Danil Andreev
     */
    Token readSymbol();

    /**
     * readNumber - method, designed to read braced string from the stream.
     * @author Danil Andreev
     */
    Token readString();

protected:
    /**
     * isCharacterSkippable - method, designed to check if the character is skippable.
     * @param character - Character for the check.
     * @author Danil Andreev
     */
    bool isCharacterSkippable(const char character);

    /**
     * getFromStream - method, designed to get multiple characters from the stream.
     * @param length - Target symbols quantity.
     * @author Danil Andreev
     */
    string getFromStream(size_t length);
};


#endif //APE_LANG_COMPILER_LEXER_H