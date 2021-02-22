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

#ifndef APE_LANG_COMPILER_TOKENIZER_H
#define APE_LANG_COMPILER_TOKENIZER_H

#include <memory>
#include "../Lexer/Lexer.h"
#include "Node.h"
#include "DeclarationNode.h"

class Tokenizer {
protected:
    Lexer *lexer;
protected:
    typedef map<string, DeclarationNode> Scope;
protected:
    static shared_ptr<Node> validateTree(const shared_ptr<Node> input, const shared_ptr<Scope> scope = nullptr);

public:
    explicit Tokenizer(Lexer *lexer);

    explicit Tokenizer(Lexer &lexer);

    Tokenizer(const Tokenizer &reference);

    ~Tokenizer();

public:
    shared_ptr<Node> parse();

protected:
    shared_ptr<Node> statement() const;

    shared_ptr<Node> parenExpr() const;

    shared_ptr<Node> argumentsDeclaration() const;

    shared_ptr<Node> arguments() const;

    shared_ptr<Node> expression() const;

    shared_ptr<DeclarationNode> declaration(bool initialization = true, bool semicolon = true) const;

    shared_ptr<Node> test() const;

    shared_ptr<Node> summa() const;

    shared_ptr<Node> term() const;
};


#endif //APE_LANG_COMPILER_TOKENIZER_H
