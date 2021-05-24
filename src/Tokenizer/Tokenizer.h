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
#include <vector>
#include <map>
#include "../Lexer/Tokens/Token.h"
#include "../Lexer/Lexerable.h"
#include "Nodes/Node.h"
#include "Nodes/VariableNode.h"
#include "../exceptions/ApeCompilerException.h"

enum RPN {
    RPN_START,
    RPN_END,
    RPN_ROUND_BRACE_OPEN,
    RPN_ROUND_BRACE_CLOSE,
    RPN_ADD,
    RPN_SUBTRACT,
    RPN_MULTIPLY,
    RPN_DIVIDE,
    RPN_POWER,
    RPN_AND,
    RPN_OR,
    RPN_GREATER,
    RPN_LESS,
    RPN_GREATER_EQUAL,
    RPN_LESS_EQUAL,
    RPN_EQUAL,
    RPN_NOTEQUAL,
    RPN_OPERAND,
};

class Tokenizer {
protected:
    Lexerable *lexer;
    static std::map<RPN, unsigned short> Priorities;
protected:
    typedef std::pair<const std::string, std::shared_ptr<VariableNode>> ScopeItem;
    typedef std::map<const std::string, std::shared_ptr<VariableNode>> Scope;
public:
    static std::tuple<
            std::shared_ptr<Node>,
            std::shared_ptr<std::vector<ApeCompilerException>>,
            VariableNode::DATA_TYPE
    > validateTree(
            const std::shared_ptr<Node> input,
            std::shared_ptr<Scope> scope = nullptr,
            std::shared_ptr<Scope> outerScope = nullptr,
            std::shared_ptr<std::vector<ApeCompilerException>> errors = nullptr
    );

public:
    explicit Tokenizer(Lexerable *lexer);

    explicit Tokenizer(Lexerable &lexer);

    Tokenizer(const Tokenizer &reference);

    ~Tokenizer();

public:
    std::shared_ptr<Node> parse();

protected:
    std::shared_ptr<Node> statement() const;

    std::shared_ptr<Node> parenExpr() const;

    std::shared_ptr<Node> argumentsDeclaration() const;

    std::shared_ptr<Node> arguments() const;

    std::shared_ptr<Node> expression() const;

    std::shared_ptr<VariableNode> declaration(bool initialization = true, bool semicolon = true) const;

    /**
     * rpn_math - parses arithmetical and logical expression.
     * @author Danil Andreev
     */
    std::shared_ptr<Node> rpn_math() const;

    std::pair<RPN, std::shared_ptr<Node>> rpn_term() const;
};


#endif //APE_LANG_COMPILER_TOKENIZER_H
