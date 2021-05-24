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

#include <iostream>
#include <set>
#include <vector>
#include <regex>
#include <string>
#include "Lexerable.h"
#include "Tokens/Token.h"
#include "Tokens/OperatorToken.h"
#include "Tokens/KeywordToken.h"

class Lexer : public Lexerable {
protected:
    /// SkippableCharacters - RegExp for skipping characters.
    static std::regex SkippableCharacters;
    /// Symbols - an array of lang symbols/operators. Will be added to set.
    static std::vector<pair<string, OPERATORS>> Symbols;
    /// Keywords - set of lang keywords.
    static std::set<pair<string, KEYWORDS>> Keywords;
    /// symbols - set of lang symbols and operators, ordered by length DESC.
    std::set<pair<string, OPERATORS>, bool (*)(const std::pair<string, OPERATORS> &,
                                               const pair<string, OPERATORS> &)> *symbols;
    /// symbolsStartCharacters - a set of first characters from symbols set for lexer.
    std::set<char> *symbolsStartCharacters;
    /// currentCharacter - last character got from the input stream.
    char currentCharacter;
protected:
    /// stream - input data stream with program code.
    std::istream &stream;
    /// Current line in code.
    int line;
    /// Current column in code.
    int column;
    std::string lineText;
    /// currentToken - last token got from the input stream.
    std::shared_ptr<Token> currentToken;
public:
    explicit Lexer(istream &stream);

    Lexer(const Lexer &reference);

    ~Lexer();

public:
    /**
     * nextToken - method, designed to get next token from input stream.
     * Also sets got token to lexer current token.
     * @return next token.
     * @author Danil Andreev
     */
    std::shared_ptr<Token> nextToken() override;

    /**
     * getCurrentToken - getter for Lexer current token.
     * @author Danil Andreev
     */
    std::shared_ptr<Token> getCurrentToken() const override;

    /**
     * isEof - returns true if eof flag has been set.
     * Else returns false.
     */
    bool isEof() const override;

protected:
    /**
     * getNextToken - method, designed to get next token from input stream.
     * @return next token.
     * @author Danil Andreev
     */
    std::shared_ptr<Token> getNextToken();

    /**
     * readNumber - method, designed to read number from the stream.
     * @author Danil Andreev
     */
    std::shared_ptr<Token> readNumber();

    /**
     * readIdentifier - method, designed to read identifier from the stream.
     * @author Danil Andreev
     */
    std::shared_ptr<Token> readIdentifier();

    /**
     * readSymbol - method, designed to read symbol/operator from the stream.
     * @author Danil Andreev
     */
    std::shared_ptr<Token> readSymbol();

    /**
     * readString - method, designed to read braced string from the stream.
     * @author Danil Andreev
     */
    std::shared_ptr<Token> readString();

protected:
    /**
     * get - gets next character from the stream.
     * @return next character.
     */
    char get();

    /**
     * get - gets next character from the stream.
     * @param input - reference to variable to write in.
     */
    void get(char &input);

    /**
     * get - gets n characters from the stream.
     * @param length - characters quantity to get.
     * @return next characters sequence.
     */
    std::string get(std::size_t length);

    /**
     * oversee - gets next n characters from the stream without extracting them.
     * @param length
     * @return next characters sequence.
     */
    std::string oversee(std::size_t length);

protected:
    /**
     * isCharacterSkippable - method, designed to check if the character is skippable.
     * @param character - Character for the check.
     * @author Danil Andreev
     */
    bool isCharacterSkipable(const char character);
};


#endif //APE_LANG_COMPILER_LEXER_H
