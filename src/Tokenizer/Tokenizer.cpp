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

#include "Tokenizer.h"
#include "../exceptions/ApeCompilerException.h"
#include "StringNode.h"
#include "IntegerNode.h"
#include "FloatNode.h"
#include "VariableNode.h"

Tokenizer::Tokenizer(Lexer *lexer) {
    this->lexer = lexer;
}

Tokenizer::Tokenizer(Lexer &lexer) : Tokenizer(&lexer) {}

Tokenizer::Tokenizer(const Tokenizer &reference) {
    this->lexer = reference.lexer;
}

shared_ptr<Node> Tokenizer::parse() {
    this->lexer->nextToken();
    shared_ptr<Node> node = make_shared<Node>(Node::PROGRAM, this->statement());
    if (!this->lexer->isEof())
        throw exception();
    return node;
}

shared_ptr<Node> Tokenizer::statement() const {
    shared_ptr<Token> entry = this->lexer->getCurrentToken();
    shared_ptr<Node> node = nullptr;

    switch (entry->getType()) {
        /// KEYWORDS
        case Token::KEYWORD: {
            shared_ptr<KeywordToken> token = dynamic_pointer_cast<KeywordToken>(entry);

            switch (token->getKeywordType()) {
                /// IF
                case KEYWORDS::IF: {
                    node = make_shared<Node>(Node::IF);
                    this->lexer->nextToken();
                    node->setOperand1(this->parenExpr());
                    node->setOperand2(this->statement());
                    entry = this->lexer->getCurrentToken();
                    if (entry->getType() == Token::KEYWORD) {
                        shared_ptr<KeywordToken> elseToken = dynamic_pointer_cast<KeywordToken>(entry);
                        /// ELSE
                        if (elseToken->getKeywordType() == KEYWORDS::ELSE) {
                            node->setType(Node::IFELSE);
                            this->lexer->nextToken();
                            node->setOperand3(this->statement());
                        }
                    }
                }
                    break;
                case KEYWORDS::WHILE: {
                    node = make_shared<Node>(Node::WHILE);
                    this->lexer->nextToken();
                    node->setOperand1(this->parenExpr());
                    node->setOperand2(this->statement());
                }
                    break;
                case KEYWORDS::DO: {
                    node = make_shared<Node>(Node::DO);
                    this->lexer->nextToken();
                    node->setOperand1(this->statement());
                    if (this->lexer->getCurrentToken()->getType() != Token::KEYWORD)
                        throw ApeCompilerException("Expected \"while\"");
                    token = dynamic_pointer_cast<KeywordToken>(this->lexer->getCurrentToken());
                    if (token->getKeywordType() != KEYWORDS::WHILE)
                        throw ApeCompilerException("Expected \"while\"");
                    this->lexer->nextToken();
                    node->setOperand2(this->parenExpr());
                    if (this->lexer->getCurrentToken()->getType() != Token::SYMBOL)
                        throw ApeCompilerException("Expected \";\"");
                    if (dynamic_pointer_cast<OperatorToken>(this->lexer->getCurrentToken())->getOperatorType() !=
                        OPERATORS::SEMICOLON)
                        throw ApeCompilerException("Expected \";\"");
                }
                    break;
                case KEYWORDS::CONST: {
                    this->lexer->nextToken();
                    node = this->declaration();
                    dynamic_pointer_cast<DeclarationNode>(node)->setConstant(true);
                }
                    break;
                default:
                    KEYWORDS type = token->getKeywordType();
                    if (
                            type == KEYWORDS::FLOAT ||
                            type == KEYWORDS::BOOLEAN ||
                            type == KEYWORDS::INT ||
                            type == KEYWORDS::STRING
                            ) {
                        return this->declaration();
                    }
            }
        }
            break;
            /// SYMBOLS
        case Token::SYMBOL: {
            shared_ptr<OperatorToken> token = dynamic_pointer_cast<OperatorToken>(entry);

            switch (token->getOperatorType()) {
                /// SEMICOLON
                case OPERATORS::SEMICOLON: {
                    node = make_shared<Node>(Node::EMPTY);
                    this->lexer->nextToken();

                }
                    break;
                    /// CURLY_BRACE_OPEN
                case OPERATORS::CURLY_BRACE_OPEN: {
                    node = make_shared<Node>(Node::EMPTY);
                    this->lexer->nextToken();

                    while (!(
                            this->lexer->getCurrentToken()->getType() == Token::SYMBOL &&
                            dynamic_pointer_cast<OperatorToken>(this->lexer->getCurrentToken())
                                    ->getOperatorType() == OPERATORS::CURLY_BRACE_CLOSE
                    )) {
                        node = make_shared<Node>(Node::SEQUENCE, node, this->statement());
                    }
                    this->lexer->nextToken();
                }
                    break;
            }
        }
            break;
        default:
            node = make_shared<Node>(Node::EXPRESSION, this->expression());
            if (this->lexer->getCurrentToken()->getType() != Token::SYMBOL)
                throw exception();
            shared_ptr<OperatorToken> token = dynamic_pointer_cast<OperatorToken>(this->lexer->getCurrentToken());
            if (token->getOperatorType() != OPERATORS::SEMICOLON)
                throw exception();
            this->lexer->nextToken();
    }

    return node;
}

shared_ptr<Node> Tokenizer::parenExpr() const {
    shared_ptr<Node> node = nullptr;

    shared_ptr<Token> token = this->lexer->getCurrentToken();
    if (token->getType() != Token::SYMBOL)
        throw exception();
    shared_ptr<OperatorToken> keyword = dynamic_pointer_cast<OperatorToken>(token);
    if (keyword->getOperatorType() != OPERATORS::ROUND_BRACE_OPEN)
        throw exception();

    this->lexer->nextToken();
    node = this->expression();

    token = this->lexer->getCurrentToken();
    if (token->getType() != Token::SYMBOL)
        throw exception();
    keyword = dynamic_pointer_cast<OperatorToken>(token);
    if (keyword->getOperatorType() != OPERATORS::ROUND_BRACE_CLOSE)
        throw exception();

    this->lexer->nextToken();
    return node;
}

shared_ptr<Node> Tokenizer::expression() const {
    shared_ptr<Node> node = this->test();

    if (this->lexer->getCurrentToken()->getType() == Token::IDENTIFIER)
        return node;

    if (node->getType() == Node::VAR && this->lexer->getCurrentToken()->getType() == Token::SYMBOL) {
        shared_ptr<OperatorToken> token = dynamic_pointer_cast<OperatorToken>(this->lexer->getCurrentToken());
        if (token->getOperatorType() == OPERATORS::ASSIGN) {
            this->lexer->nextToken();
            node = make_shared<Node>(Node::SET, node, this->summa());
        }
    }

    return node;
}

shared_ptr<Node> Tokenizer::test() const {
    shared_ptr<Node> node = this->summa();

    shared_ptr<Token> token = this->lexer->getCurrentToken();
    if (token->getType() == Token::SYMBOL) {
        shared_ptr<OperatorToken> operatorToken = dynamic_pointer_cast<OperatorToken>(token);
        switch (operatorToken->getOperatorType()) {
            case OPERATORS::LESS:
                this->lexer->nextToken();
                node = make_shared<Node>(Node::LESS, node, this->summa());
                break;
            case OPERATORS::GREATER:
                this->lexer->nextToken();
                node = make_shared<Node>(Node::GREATER, node, this->summa());
                break;
            case OPERATORS::LESS_EQUAL:
                this->lexer->nextToken();
                node = make_shared<Node>(Node::LESS_EQUAL, node, this->summa());
                break;
            case OPERATORS::GREATER_EQUAL:
                this->lexer->nextToken();
                node = make_shared<Node>(Node::GREATER_EQUAL, node, this->summa());
                break;
            case OPERATORS::EQUAL:
                this->lexer->nextToken();
                node = make_shared<Node>(Node::EQUAL, node, this->summa());
                break;
            case OPERATORS::NOTEQUAL:
                this->lexer->nextToken();
                node = make_shared<Node>(Node::NOT_EQUAL, node, this->summa());
                break;
        }
    }

    return node;
}

shared_ptr<Node> Tokenizer::summa() const {
    shared_ptr<Node> node = this->term();

    shared_ptr<OperatorToken> token;
    while (
            (token = dynamic_pointer_cast<OperatorToken>(this->lexer->getCurrentToken())) != nullptr &&
            (
                    token->getOperatorType() == OPERATORS::ADD ||
                    token->getOperatorType() == OPERATORS::SUBTRACT ||
                    token->getOperatorType() == OPERATORS::MULTIPLY ||
                    token->getOperatorType() == OPERATORS::DIVIDE
            )
            ) {
        Node::TYPE type;
        switch (token->getOperatorType()) {
            case OPERATORS::ADD:
                type = Node::ADD;
                break;
            case OPERATORS::SUBTRACT:
                type = Node::SUBTRACT;
                break;
            case OPERATORS::MULTIPLY:
                type = Node::MULTIPLY;
                break;
            case OPERATORS::DIVIDE:
                type = Node::DIVIDE;
                break;
        }
        this->lexer->nextToken();
        node = make_shared<Node>(type, node, this->term());

    }

    return node;
}

shared_ptr<Node> Tokenizer::term() const {
    shared_ptr<Node> node = nullptr;
    switch (this->lexer->getCurrentToken()->getType()) {
        case Token::IDENTIFIER : {
            string payload = this->lexer->getCurrentToken()->getPayload();
            node = make_shared<VariableNode>(payload, false);
            shared_ptr<Token> token = this->lexer->nextToken();
            if (token->getType() == Token::SYMBOL) {
                shared_ptr<OperatorToken> opToken = dynamic_pointer_cast<OperatorToken>(token);
                switch (opToken->getOperatorType()) {
                    case OPERATORS::ROUND_BRACE_OPEN: {
                        dynamic_pointer_cast<VariableNode>(node)->setIsFunction(true);
                        node->setOperand1(this->arguments());
                    }
                        break;
                }
            }
        }
            break;
        case Token::NUMBER: {
            shared_ptr<NumberToken> token = dynamic_pointer_cast<NumberToken>(this->lexer->getCurrentToken());
            if (token->isInteger())
                node = make_shared<IntegerNode>(token->getLong());
            else
                node = make_shared<FloatNode>(token->getDouble());

            this->lexer->nextToken();
        }
            break;
        case Token::STRING: {
            string payload = this->lexer->getCurrentToken()->getPayload();
            node = make_shared<StringNode>(payload);
            this->lexer->nextToken();
        }
            break;
        default:
            return this->parenExpr();
    }

    return node;
}

shared_ptr<DeclarationNode> Tokenizer::declaration(bool initialization, bool semicolon) const {
    shared_ptr<DeclarationNode> node = nullptr;
    shared_ptr<Token> token = this->lexer->getCurrentToken();
    if (token->getType() != Token::KEYWORD)
        throw ApeCompilerException("Incorrect type");

    shared_ptr<KeywordToken> keywordToken = dynamic_pointer_cast<KeywordToken>(token);
    if (keywordToken == nullptr)
        throw ApeCompilerException("Invalid cast");

    KEYWORDS type = keywordToken->getKeywordType();

    DeclarationNode::DATA_TYPE dataType;
    switch (type) {
        case KEYWORDS::FLOAT:
            dataType = DeclarationNode::DATA_TYPE::FLOAT;
            break;
        case KEYWORDS::INT:
            dataType = DeclarationNode::DATA_TYPE::INT;
            break;
        case KEYWORDS::BOOLEAN:
            dataType = DeclarationNode::DATA_TYPE::BOOLEAN;
            break;
        case KEYWORDS::STRING:
            dataType = DeclarationNode::DATA_TYPE::STRING;
            break;
    }
    shared_ptr<Token> current = this->lexer->nextToken();
    if (current->getType() != Token::IDENTIFIER)
        throw ApeCompilerException("Incorrect daclaration");
    string name = current->getPayload();
    node = make_shared<DeclarationNode>(dataType, name);
    current = this->lexer->nextToken();
    if (initialization && current->getType() == Token::SYMBOL) {
        shared_ptr<OperatorToken> opToken = dynamic_pointer_cast<OperatorToken>(current);
        if (opToken->getOperatorType() == OPERATORS::ASSIGN) {
            this->lexer->nextToken();
            node->setOperand1(this->test());
        } else if (opToken->getOperatorType() == OPERATORS::ROUND_BRACE_OPEN) {

            node->setOperand1(this->argumentsDeclaration());
            node->setOperand2(this->statement());
            return node;
        }
    }
    if (semicolon) {
        current = this->lexer->getCurrentToken();
        if (current->getType() != Token::SYMBOL)
            throw ApeCompilerException("Expected \";\"");
        if (dynamic_pointer_cast<OperatorToken>(current)->getOperatorType() != OPERATORS::SEMICOLON)
            throw ApeCompilerException("Expected \";\"");
    }

    return node;
}

shared_ptr<Node> Tokenizer::argumentsDeclaration() const {
    shared_ptr<Token> token = this->lexer->getCurrentToken();
    if (token->getType() != Token::SYMBOL)
        throw ApeCompilerException("Expected \"(\"");
    shared_ptr<OperatorToken> opToken = dynamic_pointer_cast<OperatorToken>(token);
    if (opToken == nullptr || opToken->getOperatorType() != OPERATORS::ROUND_BRACE_OPEN)
        throw ApeCompilerException("Expected \"(\"");
    token = this->lexer->nextToken();

    shared_ptr<Node> node = make_shared<Node>(Node::SEQUENCE);

    if (token->getType() != Token::SYMBOL) {
        opToken = make_shared<OperatorToken>(OPERATORS::COMA);
        while (opToken != nullptr && opToken->getOperatorType() == OPERATORS::COMA) {
            node = make_shared<Node>(Node::SEQUENCE, node, this->declaration(false, false));
            token = this->lexer->getCurrentToken();
            opToken = dynamic_pointer_cast<OperatorToken>(token);
            if (opToken && opToken->getOperatorType() == OPERATORS::COMA)
                this->lexer->nextToken();
        }
    }


    token = this->lexer->getCurrentToken();
    if (token->getType() != Token::SYMBOL)
        throw ApeCompilerException("Expected \")\"");
    opToken = dynamic_pointer_cast<OperatorToken>(token);
    if (opToken == nullptr || opToken->getOperatorType() != OPERATORS::ROUND_BRACE_CLOSE)
        throw ApeCompilerException("Expected \")\"");
    this->lexer->nextToken();
    return node;
}

shared_ptr<Node> Tokenizer::arguments() const {
    shared_ptr<Token> token = this->lexer->getCurrentToken();
    if (token->getType() != Token::SYMBOL)
        throw ApeCompilerException("Expected \"(\"");
    shared_ptr<OperatorToken> opToken = dynamic_pointer_cast<OperatorToken>(token);
    if (opToken == nullptr || opToken->getOperatorType() != OPERATORS::ROUND_BRACE_OPEN)
        throw ApeCompilerException("Expected \"(\"");
    token = this->lexer->nextToken();

    shared_ptr<Node> node = make_shared<Node>(Node::SEQUENCE);

    if (token->getType() != Token::SYMBOL) {
        opToken = make_shared<OperatorToken>(OPERATORS::COMA);
        while (opToken != nullptr && opToken->getOperatorType() == OPERATORS::COMA) {
            node = make_shared<Node>(Node::SEQUENCE, node, this->test());
            token = this->lexer->getCurrentToken();
            opToken = dynamic_pointer_cast<OperatorToken>(token);
            if (opToken && opToken->getOperatorType() == OPERATORS::COMA)
                this->lexer->nextToken();
        }
    }


    token = this->lexer->getCurrentToken();
    if (token->getType() != Token::SYMBOL)
        throw ApeCompilerException("Expected \")\"");
    opToken = dynamic_pointer_cast<OperatorToken>(token);
    if (opToken == nullptr || opToken->getOperatorType() != OPERATORS::ROUND_BRACE_CLOSE)
        throw ApeCompilerException("Expected \")\"");
    this->lexer->nextToken();
    return node;
}
