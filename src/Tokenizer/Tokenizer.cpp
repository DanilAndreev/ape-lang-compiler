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

#include <stack>
#include "Tokenizer.h"
#include "Nodes/StringNode.h"
#include "Nodes/IntegerNode.h"
#include "Nodes/FloatNode.h"
#include "Nodes/BooleanNode.h"
#include "Nodes/ConvertNode.h"
#include "../Lexer/Tokens/KeywordToken.h"
#include "../Lexer/Tokens/OperatorToken.h"
#include "../Lexer/Tokens/NumberToken.h"
#include "../exceptions/CodeException.h"
#include "../exceptions/UnexpectedTokenException.h"
#include "../exceptions/ReDeclarationException.h"

using namespace std;

map<RPN, unsigned short> Tokenizer::Priorities = {
        {RPN_START,             0},
        {RPN_ROUND_BRACE_OPEN,  1},
        {RPN_ROUND_BRACE_CLOSE, 1},
        {RPN_ADD,               2},
        {RPN_SUBTRACT,          2},
        {RPN_MULTIPLY,          3},
        {RPN_DIVIDE,            3},
        {RPN_POWER,             4},
        {RPN_AND,               5},
        {RPN_OR,                5},
        {RPN_GREATER,           6},
        {RPN_LESS,              6},
        {RPN_GREATER_EQUAL,     6},
        {RPN_LESS_EQUAL,        6},
        {RPN_EQUAL,             6},
        {RPN_NOTEQUAL,          6},
};

Tokenizer::Tokenizer(Lexerable *lexer) {
    this->lexer = lexer;
}

Tokenizer::Tokenizer(Lexerable &lexer) : Tokenizer(&lexer) {}

Tokenizer::Tokenizer(const Tokenizer &reference) {
    this->lexer = reference.lexer;
}

Tokenizer::~Tokenizer() {}

shared_ptr<Node> Tokenizer::parse() {
    shared_ptr<Token> token = this->lexer->nextToken();
    shared_ptr<Node> node = make_shared<Node>(token->getLine(), token->getColumn(), Node::PROGRAM, this->statement());
    if (!this->lexer->isEof())
        throw ApeCompilerException("Missing end of file");
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
                    node = make_shared<Node>(token->getLine(), token->getColumn(), Node::IF);
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
                    node = make_shared<Node>(token->getLine(), token->getColumn(), Node::WHILE);
                    this->lexer->nextToken();
                    node->setOperand1(this->parenExpr());
                    node->setOperand2(this->statement());
                }
                    break;
                case KEYWORDS::FOR: {
                    shared_ptr<Node> forNode = make_shared<Node>(token->getLine(), token->getColumn(), Node::FOR);
                    node = make_shared<Node>(-1, -1, Node::SEQUENCE);
                    node->setOperand2(forNode);

                    this->lexer->nextToken();
                    if (this->lexer->getCurrentToken()->getType() != Token::SYMBOL)
                        throw UnexpectedTokenException(this->lexer->getCurrentToken(), "\"(\"");
                    if (dynamic_pointer_cast<OperatorToken>(this->lexer->getCurrentToken())->getOperatorType() !=
                        OPERATORS::ROUND_BRACE_OPEN)
                        throw UnexpectedTokenException(this->lexer->getCurrentToken(), "\"(\"");


                    if (this->lexer->nextToken()->getType() == Token::KEYWORD) {
                        node->setOperand1(this->declaration());
                    } else {
                        node->setOperand1(this->expression());
                    }

                    if (this->lexer->getCurrentToken()->getType() != Token::SYMBOL)
                        throw UnexpectedTokenException(this->lexer->getCurrentToken(), "\";\"");
                    if (dynamic_pointer_cast<OperatorToken>(this->lexer->getCurrentToken())->getOperatorType() !=
                        OPERATORS::SEMICOLON)
                        throw UnexpectedTokenException(this->lexer->getCurrentToken(), "\";\"");


                    this->lexer->nextToken();
                    forNode->setOperand1(this->rpn_math());

                    if (this->lexer->getCurrentToken()->getType() != Token::SYMBOL)
                        throw UnexpectedTokenException(this->lexer->getCurrentToken(), "\";\"");
                    if (dynamic_pointer_cast<OperatorToken>(this->lexer->getCurrentToken())->getOperatorType() !=
                        OPERATORS::SEMICOLON)
                        throw UnexpectedTokenException(this->lexer->getCurrentToken(), "\";\"");

                    this->lexer->nextToken();
                    forNode->setOperand2(this->expression());

                    if (this->lexer->getCurrentToken()->getType() != Token::SYMBOL)
                        throw UnexpectedTokenException(this->lexer->getCurrentToken(), "\")\"");
                    if (dynamic_pointer_cast<OperatorToken>(this->lexer->getCurrentToken())->getOperatorType() !=
                        OPERATORS::ROUND_BRACE_CLOSE)
                        throw UnexpectedTokenException(this->lexer->getCurrentToken(), "\")\"");

                    this->lexer->nextToken();
                    forNode->setOperand3(this->statement());
                    node = make_shared<Node>(
                            this->lexer->getCurrentToken()->getLine(),
                            this->lexer->getCurrentToken()->getColumn(),
                            Node::SCOPE, node
                    );
                }
                    break;
                case KEYWORDS::DO: {
                    node = make_shared<Node>(token->getLine(), token->getColumn(), Node::DO);
                    this->lexer->nextToken();
                    node->setOperand1(this->statement());
                    if (this->lexer->getCurrentToken()->getType() != Token::KEYWORD)
                        throw UnexpectedTokenException(this->lexer->getCurrentToken(), "\"while\"");
                    token = dynamic_pointer_cast<KeywordToken>(this->lexer->getCurrentToken());
                    if (token->getKeywordType() != KEYWORDS::WHILE)
                        throw UnexpectedTokenException(this->lexer->getCurrentToken(), "\"while\"");
                    this->lexer->nextToken();
                    node->setOperand2(this->parenExpr());
                    if (this->lexer->getCurrentToken()->getType() != Token::SYMBOL)
                        throw UnexpectedTokenException(this->lexer->getCurrentToken(), "\";\"");
                    if (dynamic_pointer_cast<OperatorToken>(this->lexer->getCurrentToken())->getOperatorType() !=
                        OPERATORS::SEMICOLON)
                        throw UnexpectedTokenException(this->lexer->getCurrentToken(), "\";\"");
                }
                    break;
                case KEYWORDS::CONST: {
                    this->lexer->nextToken();
                    node = this->declaration();
                    dynamic_pointer_cast<VariableNode>(node)->setConstant(true);
                }
                    break;
                case KEYWORDS::PRINT: {
                    shared_ptr<Token> printToken = this->lexer->nextToken();
                    if (this->lexer->getCurrentToken()->getType() != Token::SYMBOL)
                        throw UnexpectedTokenException(this->lexer->getCurrentToken(), "\"(\"");
                    if (dynamic_pointer_cast<OperatorToken>(this->lexer->getCurrentToken())->getOperatorType() !=
                        OPERATORS::ROUND_BRACE_OPEN)
                        throw UnexpectedTokenException(this->lexer->getCurrentToken(), "\"(\"");

                    this->lexer->nextToken();
                    node = this->rpn_math();

                    if (this->lexer->getCurrentToken()->getType() != Token::SYMBOL)
                        throw UnexpectedTokenException(this->lexer->getCurrentToken(), "\")\"");
                    if (dynamic_pointer_cast<OperatorToken>(this->lexer->getCurrentToken())->getOperatorType() !=
                        OPERATORS::ROUND_BRACE_CLOSE)
                        throw UnexpectedTokenException(this->lexer->getCurrentToken(), "\")\"");

                    this->lexer->nextToken();
                    if (this->lexer->getCurrentToken()->getType() != Token::SYMBOL)
                        throw UnexpectedTokenException(this->lexer->getCurrentToken(), "\";\"");
                    if (dynamic_pointer_cast<OperatorToken>(this->lexer->getCurrentToken())->getOperatorType() !=
                        OPERATORS::SEMICOLON)
                        throw UnexpectedTokenException(this->lexer->getCurrentToken(), "\";\"");
                    node = make_shared<Node>(printToken->getLine(), printToken->getColumn(), Node::PRINT, node);

                    this->lexer->nextToken();
                }
                    break;
                case KEYWORDS::READ: {
                    shared_ptr<Token> readToken = this->lexer->nextToken();
                    if (this->lexer->getCurrentToken()->getType() != Token::SYMBOL)
                        throw UnexpectedTokenException(this->lexer->getCurrentToken(), "\"(\"");
                    if (dynamic_pointer_cast<OperatorToken>(this->lexer->getCurrentToken())->getOperatorType() !=
                        OPERATORS::ROUND_BRACE_OPEN)
                        throw UnexpectedTokenException(this->lexer->getCurrentToken(), "\"(\"");

                    this->lexer->nextToken();
                    if (this->lexer->getCurrentToken()->getType() != Token::IDENTIFIER)
                        throw UnexpectedTokenException(this->lexer->getCurrentToken(), "identifier");

                    string varName = this->lexer->getCurrentToken()->getPayload();
                    node = make_shared<VariableNode>(
                            this->lexer->getCurrentToken()->getLine(),
                            this->lexer->getCurrentToken()->getColumn(),
                            varName,
                            false);

                    this->lexer->nextToken();
                    if (this->lexer->getCurrentToken()->getType() != Token::SYMBOL)
                        throw UnexpectedTokenException(this->lexer->getCurrentToken(), "\")\"");
                    if (dynamic_pointer_cast<OperatorToken>(this->lexer->getCurrentToken())->getOperatorType() !=
                        OPERATORS::ROUND_BRACE_CLOSE)
                        throw UnexpectedTokenException(this->lexer->getCurrentToken(), "\")\"");

                    this->lexer->nextToken();
                    if (this->lexer->getCurrentToken()->getType() != Token::SYMBOL)
                        throw UnexpectedTokenException(this->lexer->getCurrentToken(), "\";\"");
                    if (dynamic_pointer_cast<OperatorToken>(this->lexer->getCurrentToken())->getOperatorType() !=
                        OPERATORS::SEMICOLON)
                        throw UnexpectedTokenException(this->lexer->getCurrentToken(), "\";\"");
                    node = make_shared<Node>(readToken->getLine(), readToken->getColumn(), Node::READ, node);
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
                    node = make_shared<Node>(token->getLine(), token->getColumn(), Node::EMPTY);
                    this->lexer->nextToken();

                }
                    break;
                    /// CURLY_BRACE_OPEN
                case OPERATORS::CURLY_BRACE_OPEN: {
                    node = make_shared<Node>(token->getLine(), token->getColumn(), Node::EMPTY);
                    this->lexer->nextToken();

                    while (!(
                            this->lexer->getCurrentToken()->getType() == Token::SYMBOL &&
                            dynamic_pointer_cast<OperatorToken>(this->lexer->getCurrentToken())
                                    ->getOperatorType() == OPERATORS::CURLY_BRACE_CLOSE
                    )) {
                        node = make_shared<Node>(this->lexer->getCurrentToken()->getLine(),
                                                 this->lexer->getCurrentToken()->getColumn(),
                                                 Node::SEQUENCE,
                                                 node,
                                                 this->statement());
                    }
                    node = make_shared<Node>(token->getLine(), token->getColumn(), Node::SCOPE, node);
                    this->lexer->nextToken();
                }
                    break;
            }
        }
            break;
        default:
            node = make_shared<Node>(
                    this->lexer->getCurrentToken()->getLine(),
                    this->lexer->getCurrentToken()->getColumn(),
                    Node::EXPRESSION,
                    this->expression()
            );
            if (this->lexer->getCurrentToken()->getType() != Token::SYMBOL)
                throw UnexpectedTokenException(this->lexer->getCurrentToken(), "\";\"");
            shared_ptr<OperatorToken> token = dynamic_pointer_cast<OperatorToken>(this->lexer->getCurrentToken());
            if (token->getOperatorType() != OPERATORS::SEMICOLON)
                throw UnexpectedTokenException(this->lexer->getCurrentToken(), "\";\"");
            this->lexer->nextToken();
    }

    return node;
}

shared_ptr<Node> Tokenizer::parenExpr() const {
    shared_ptr<Node> node = nullptr;

    shared_ptr<Token> token = this->lexer->getCurrentToken();
    if (token->getType() != Token::SYMBOL)
        throw UnexpectedTokenException(this->lexer->getCurrentToken(), "\"(\"");
    shared_ptr<OperatorToken> keyword = dynamic_pointer_cast<OperatorToken>(token);
    if (keyword->getOperatorType() != OPERATORS::ROUND_BRACE_OPEN)
        throw UnexpectedTokenException(this->lexer->getCurrentToken(), "\"(\"");

    this->lexer->nextToken();
    node = this->expression();

    token = this->lexer->getCurrentToken();
    if (token->getType() != Token::SYMBOL)
        throw UnexpectedTokenException(this->lexer->getCurrentToken(), "\")\"");
    keyword = dynamic_pointer_cast<OperatorToken>(token);
    if (keyword->getOperatorType() != OPERATORS::ROUND_BRACE_CLOSE)
        throw UnexpectedTokenException(this->lexer->getCurrentToken(), "\")\"");

    this->lexer->nextToken();
    return node;
}

shared_ptr<Node> Tokenizer::expression() const {
    shared_ptr<Node> node = this->rpn_math();

    if (this->lexer->getCurrentToken()->getType() == Token::IDENTIFIER)
        return node;

    if (node->getType() == Node::VAR && this->lexer->getCurrentToken()->getType() == Token::SYMBOL) {
        shared_ptr<OperatorToken> token = dynamic_pointer_cast<OperatorToken>(this->lexer->getCurrentToken());
        if (token->getOperatorType() == OPERATORS::ASSIGN) {
            shared_ptr<Token> setToken = this->lexer->nextToken();
            node = make_shared<Node>(setToken->getLine(), setToken->getColumn(), Node::SET, node, this->rpn_math());
        }
    }

    return node;
}

shared_ptr<Node> Tokenizer::rpn_math() const {
    vector<pair<RPN, shared_ptr<Node>>> result;
    stack<pair<RPN, shared_ptr<Node>>> stc;
    stc.push(pair(RPN::RPN_START, nullptr));
    vector<pair<RPN, shared_ptr<Node>>> inputTokens;

    int braces = 0;
    while (true) {
        shared_ptr<OperatorToken> op = dynamic_pointer_cast<OperatorToken>(this->lexer->getCurrentToken());
        if (op != nullptr) {
            if (op->getOperatorType() == OPERATORS::ROUND_BRACE_OPEN) {
                braces++;
            }
            if (op->getOperatorType() == OPERATORS::ROUND_BRACE_CLOSE) {
                braces--;
                if (braces < 0) break;
            }
            if (op->getOperatorType() == OPERATORS::ASSIGN) break;
            if (op->getOperatorType() == OPERATORS::COMA) break;
            if (op->getOperatorType() == OPERATORS::SEMICOLON) break;
        }

        inputTokens.emplace_back(this->rpn_term());
    }
    inputTokens.emplace_back(pair(RPN::RPN_END, nullptr));


    while (!(inputTokens.front().first == RPN::RPN_END && stc.top().first == RPN::RPN_START)) {
        pair<RPN, shared_ptr<Node>> inputItem = inputTokens.front();

        if (inputItem.first == RPN::RPN_END) {
            result.push_back(stc.top());
            stc.pop();
            continue;
        }

        if (inputItem.first == RPN::RPN_OPERAND) {
            result.push_back(inputItem);
            inputTokens.erase(inputTokens.begin());
            continue;
        }

        pair<RPN, shared_ptr<Node>> stackItem = stc.top();

        if (stackItem.first == RPN::RPN_ROUND_BRACE_OPEN && inputItem.first == RPN::RPN_ROUND_BRACE_CLOSE) {
            inputTokens.erase(inputTokens.begin());
            stc.pop();
        } else if (stackItem.first == RPN::RPN_START && inputItem.first == RPN::RPN_ROUND_BRACE_CLOSE) {
            throw ApeCompilerException("Incorrect arithmetical expression.");  //TODO: finish errors
        } else if (stackItem.first == RPN::RPN_ROUND_BRACE_OPEN && inputTokens.empty()) {
            throw ApeCompilerException("Incorrect arithmetical expression.");
        } else {
            if (inputItem.first == RPN::RPN_ROUND_BRACE_OPEN) {
                stc.push(inputItem);
                inputTokens.erase(inputTokens.begin());
            } else if (Priorities[stackItem.first] < Priorities[inputItem.first]) {
                stc.push(inputItem);
                inputTokens.erase(inputTokens.begin());
            } else {
                result.push_back(stc.top());
                stc.pop();
            }
        }
    }

    while (!stc.empty())
        stc.pop();

    while (!result.empty()) {
        pair<RPN, shared_ptr<Node>> item = result.front();
        result.erase(result.begin());
        if (item.first == RPN::RPN_OPERAND) {
            stc.push(item);
        } else {
            pair<RPN, shared_ptr<Node>> first = stc.top();
            stc.pop();
            pair<RPN, shared_ptr<Node>> second = stc.top();
            stc.pop();
            item.second->setOperand1(second.second);
            item.second->setOperand2(first.second);
            stc.push(item);
        }
    }

    return stc.top().second;
}

shared_ptr<VariableNode> Tokenizer::declaration(bool initialization, bool semicolon) const {
    shared_ptr<VariableNode> node = nullptr;
    shared_ptr<Token> token = this->lexer->getCurrentToken();
    if (token->getType() != Token::KEYWORD)
        throw UnexpectedTokenException(this->lexer->getCurrentToken(), "data type keyword");

    shared_ptr<KeywordToken> keywordToken = dynamic_pointer_cast<KeywordToken>(token);
    if (keywordToken == nullptr)
        throw ApeCompilerException("Invalid cast");

    KEYWORDS type = keywordToken->getKeywordType();

    VariableNode::DATA_TYPE dataType;
    switch (type) {
        case KEYWORDS::FLOAT:
            dataType = VariableNode::DATA_TYPE::FLOAT;
            break;
        case KEYWORDS::INT:
            dataType = VariableNode::DATA_TYPE::INT;
            break;
        case KEYWORDS::BOOLEAN:
            dataType = VariableNode::DATA_TYPE::BOOLEAN;
            break;
        case KEYWORDS::STRING:
            dataType = VariableNode::DATA_TYPE::STRING;
            break;
        default:
            throw UnexpectedTokenException(this->lexer->getCurrentToken(), "data type keyword");
    }
    shared_ptr<Token> current = this->lexer->nextToken();
    if (current->getType() != Token::IDENTIFIER)
        throw UnexpectedTokenException(this->lexer->getCurrentToken(), "identifier");
    string name = current->getPayload();
    node = make_shared<VariableNode>(current->getLine(), current->getColumn(), name, false);
    node->setBasicType(dataType);
    node->setDeclaration(true);
    current = this->lexer->nextToken();
    if (initialization && current->getType() == Token::SYMBOL) {
        shared_ptr<OperatorToken> opToken = dynamic_pointer_cast<OperatorToken>(current);
        if (opToken->getOperatorType() == OPERATORS::ASSIGN) {
            this->lexer->nextToken();
            node->setOperand1(this->rpn_math());
        } else if (opToken->getOperatorType() == OPERATORS::ROUND_BRACE_OPEN) {
            node->setOperand1(this->argumentsDeclaration()); //TODO: swap
            node->setOperand2(this->statement());
            node->setIsFunction(true);
            return node;
        }
    }
    if (semicolon) {
        current = this->lexer->getCurrentToken();
        if (current->getType() != Token::SYMBOL)
            throw UnexpectedTokenException(this->lexer->getCurrentToken(), "\";\"");
        if (dynamic_pointer_cast<OperatorToken>(current)->getOperatorType() != OPERATORS::SEMICOLON)
            throw UnexpectedTokenException(this->lexer->getCurrentToken(), "\";\"");
    }

    return node;
}

shared_ptr<Node> Tokenizer::argumentsDeclaration() const {
    shared_ptr<Token> token = this->lexer->getCurrentToken();
    if (token->getType() != Token::SYMBOL)
        throw UnexpectedTokenException(this->lexer->getCurrentToken(), "\"(\"");
    shared_ptr<OperatorToken> opToken = dynamic_pointer_cast<OperatorToken>(token);
    if (opToken == nullptr || opToken->getOperatorType() != OPERATORS::ROUND_BRACE_OPEN)
        throw UnexpectedTokenException(this->lexer->getCurrentToken(), "\"(\"");
    token = this->lexer->nextToken();

    shared_ptr<Node> node = make_shared<Node>(token->getLine(), token->getColumn(), Node::EMPTY);

    if (token->getType() != Token::SYMBOL) {
        opToken = make_shared<OperatorToken>(OPERATORS::COMA);
        while (opToken != nullptr && opToken->getOperatorType() == OPERATORS::COMA) {
            node = make_shared<Node>(this->lexer->getCurrentToken()->getLine(),
                                     this->lexer->getCurrentToken()->getColumn(),
                                     Node::SEQUENCE,
                                     node,
                                     this->declaration(false, false));
            token = this->lexer->getCurrentToken();
            opToken = dynamic_pointer_cast<OperatorToken>(token);
            if (opToken && opToken->getOperatorType() == OPERATORS::COMA)
                this->lexer->nextToken();
        }
    }


    token = this->lexer->getCurrentToken();
    if (token->getType() != Token::SYMBOL)
        throw UnexpectedTokenException(this->lexer->getCurrentToken(), "\")\"");
    opToken = dynamic_pointer_cast<OperatorToken>(token);
    if (opToken == nullptr || opToken->getOperatorType() != OPERATORS::ROUND_BRACE_CLOSE)
        throw UnexpectedTokenException(this->lexer->getCurrentToken(), "\")\"");
    this->lexer->nextToken();
    return node;
}

shared_ptr<Node> Tokenizer::arguments() const {
    shared_ptr<Token> token = this->lexer->getCurrentToken();
    if (token->getType() != Token::SYMBOL)
        throw UnexpectedTokenException(this->lexer->getCurrentToken(), "\"(\"");
    shared_ptr<OperatorToken> opToken = dynamic_pointer_cast<OperatorToken>(token);
    if (opToken == nullptr || opToken->getOperatorType() != OPERATORS::ROUND_BRACE_OPEN)
        throw UnexpectedTokenException(this->lexer->getCurrentToken(), "\"(\"");
    token = this->lexer->nextToken();

    shared_ptr<Node> node = make_shared<Node>(this->lexer->getCurrentToken()->getLine(),
                                              this->lexer->getCurrentToken()->getColumn(),
                                              Node::EMPTY);

    if (token->getType() != Token::SYMBOL) {
        opToken = make_shared<OperatorToken>(OPERATORS::COMA);
        while (opToken != nullptr && opToken->getOperatorType() == OPERATORS::COMA) {
            node = make_shared<Node>(this->lexer->getCurrentToken()->getLine(),
                                     this->lexer->getCurrentToken()->getColumn(),
                                     Node::SEQUENCE,
                                     node,
                                     this->rpn_math());
            token = this->lexer->getCurrentToken();
            opToken = dynamic_pointer_cast<OperatorToken>(token);
            if (opToken && opToken->getOperatorType() == OPERATORS::COMA)
                this->lexer->nextToken();
        }
    }


    token = this->lexer->getCurrentToken();
    if (token->getType() != Token::SYMBOL)
        throw UnexpectedTokenException(this->lexer->getCurrentToken(), "\")\"");
    opToken = dynamic_pointer_cast<OperatorToken>(token);
    if (opToken == nullptr || opToken->getOperatorType() != OPERATORS::ROUND_BRACE_CLOSE)
        throw UnexpectedTokenException(this->lexer->getCurrentToken(), "\")\"");
    this->lexer->nextToken();
    return node;
}

tuple<
        shared_ptr<Node>,
        shared_ptr<vector<std::shared_ptr<ApeCompilerException>>>,
        VariableNode::DATA_TYPE
> Tokenizer::validateTree(
        const shared_ptr<Node> input,
        shared_ptr<Scope> scope,
        shared_ptr<Scope> outerScope,
        shared_ptr<vector<std::shared_ptr<ApeCompilerException>>> errors
) {
    if (scope == nullptr)
        scope = make_shared<Scope>();
    if (outerScope == nullptr)
        outerScope = make_shared<Scope>();
    if (errors == nullptr)
        errors = make_shared<vector<std::shared_ptr<ApeCompilerException>>>();
    VariableNode::DATA_TYPE resultDataType = VariableNode::DATA_TYPE::VOID;

    const shared_ptr<Node> operand1 = input->getOperand1();
    const shared_ptr<Node> operand2 = input->getOperand2();
    const shared_ptr<Node> operand3 = input->getOperand3();

    switch (input->getType()) {
        case Node::CONST: {
            if (dynamic_pointer_cast<FloatNode>(input)) resultDataType = VariableNode::DATA_TYPE::FLOAT;
            if (dynamic_pointer_cast<IntegerNode>(input)) resultDataType = VariableNode::DATA_TYPE::INT;
            if (dynamic_pointer_cast<BooleanNode>(input)) resultDataType = VariableNode::DATA_TYPE::BOOLEAN;
            if (dynamic_pointer_cast<StringNode>(input)) resultDataType = VariableNode::DATA_TYPE::STRING;
        }
            break;
        case Node::MULTIPLY:
        case Node::DIVIDE:
        case Node::ADD:
        case Node::SUBTRACT:
        case Node::POWER: {
            VariableNode::DATA_TYPE op1DataType = get<2>(validateTree(operand1, scope, outerScope, errors));
            VariableNode::DATA_TYPE op2DataType = get<2>(validateTree(operand2, scope, outerScope, errors));
            if (op1DataType != op2DataType) {
                input->setOperand2(
                        make_shared<ConvertNode>(operand2->getLine(), operand2->getColumn(), op1DataType, operand2));
            }
            resultDataType = op1DataType;
        }
            break;
        case Node::SCOPE: {
            shared_ptr<Scope> mergedScope = make_shared<Scope>(*outerScope);
            mergedScope->insert(scope->begin(), scope->end());

            if (operand1 != nullptr)
                validateTree(operand1, make_shared<Scope>(), mergedScope, errors);
            if (operand2 != nullptr)
                validateTree(operand2, make_shared<Scope>(), mergedScope, errors);
            if (operand3 != nullptr)
                validateTree(operand3, make_shared<Scope>(), mergedScope, errors);
        }
            break;
        case Node::VAR: {
            shared_ptr<VariableNode> variable = dynamic_pointer_cast<VariableNode>(input);
            if (!variable) throw ApeCompilerException("Invalid cast");
            if (variable->isDeclaration()) {
                auto declaration = scope->find(variable->getIdentifier());
                if (declaration != scope->end()) {
                    errors->push_back(make_shared<ReDeclarationException>(variable, declaration->second));
                } else {
                    variable->setIndex();
                    scope->insert(ScopeItem(variable->getIdentifier(), variable));
                }
                if (operand1 != nullptr) {
                    VariableNode::DATA_TYPE valueDataType = get<2>(validateTree(operand1, scope, outerScope, errors));
                    if (valueDataType != variable->getBasicType()) {
                        input->setOperand1(make_shared<ConvertNode>(operand1->getLine(), operand1->getColumn(),
                                                                    variable->getBasicType(), operand1));
                    }
                }
            } else {
                auto declaration = scope->find(variable->getIdentifier());
                auto outerDeclaration = outerScope->find(variable->getIdentifier());
                if (declaration != scope->end()) {
                    variable->setBasicType(declaration->second->getBasicType());
                    variable->setIndex(declaration->second->getIndex());
                    variable->setConstant(declaration->second->isConstant());
                } else if (outerDeclaration != outerScope->end()) {
                    variable->setBasicType(outerDeclaration->second->getBasicType());
                    variable->setIndex(outerDeclaration->second->getIndex());
                    variable->setConstant(outerDeclaration->second->isConstant());
                } else {
                    errors->push_back(make_shared<ApeCompilerException>("Undeclared variable " + variable->getIdentifier()));
                }
            }
        }
            break;
        case Node::SET: {
            validateTree(operand1, scope, outerScope, errors);
            VariableNode::DATA_TYPE valueDataType = get<2>(validateTree(operand2, scope, outerScope, errors));
            shared_ptr<VariableNode> variable = dynamic_pointer_cast<VariableNode>(input->getOperand1());
            if (variable->isConstant()) {
                errors->push_back(
                        make_shared<ApeCompilerException>("Assigning to constant variable " + variable->getIdentifier()));
            }
            if (variable->getBasicType() != valueDataType) {
                input->setOperand2(
                        make_shared<ConvertNode>(operand2->getLine(), operand2->getColumn(), variable->getBasicType(),
                                                 operand2));
            }
        }
            break;
        default:
            if (operand1 != nullptr) validateTree(operand1, scope, outerScope, errors);
            if (operand2 != nullptr) validateTree(operand2, scope, outerScope, errors);
            if (operand3 != nullptr) validateTree(operand3, scope, outerScope, errors);

    }

    return {input, errors, resultDataType};
}

pair<RPN, shared_ptr<Node>> Tokenizer::rpn_term() const {
    shared_ptr<Node> node = nullptr;
    RPN type = RPN::RPN_OPERAND;
    switch (this->lexer->getCurrentToken()->getType()) {
        case Token::KEYWORD: {
            shared_ptr<KeywordToken> keyword = dynamic_pointer_cast<KeywordToken>(this->lexer->getCurrentToken());
            if (keyword->getKeywordType() == KEYWORDS::TRUE) {
                node = make_shared<BooleanNode>(this->lexer->getCurrentToken()->getLine(),
                                                this->lexer->getCurrentToken()->getColumn(),
                                                true);
            } else if (keyword->getKeywordType() == KEYWORDS::FALSE) {
                node = make_shared<BooleanNode>(this->lexer->getCurrentToken()->getLine(),
                                                this->lexer->getCurrentToken()->getColumn(),
                                                false);
            } else {
                throw UnexpectedTokenException(this->lexer->getCurrentToken(), "term");
            }
            type = RPN::RPN_OPERAND;
            this->lexer->nextToken();
        }
            break;
        case Token::IDENTIFIER : {
            string payload = this->lexer->getCurrentToken()->getPayload();
            node = make_shared<VariableNode>(this->lexer->getCurrentToken()->getLine(),
                                             this->lexer->getCurrentToken()->getColumn(),
                                             payload,
                                             false);
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
            type = RPN::RPN_OPERAND;
        }
            break;
        case Token::NUMBER: {
            shared_ptr<NumberToken> token = dynamic_pointer_cast<NumberToken>(this->lexer->getCurrentToken());
            if (token->isInteger())
                node = make_shared<IntegerNode>(token->getLine(), token->getColumn(), token->getLong());
            else
                node = make_shared<FloatNode>(token->getLine(), token->getColumn(), token->getDouble());

            this->lexer->nextToken();
            type = RPN::RPN_OPERAND;
        }
            break;
        case Token::STRING: {
            string payload = this->lexer->getCurrentToken()->getPayload();
            node = make_shared<StringNode>(this->lexer->getCurrentToken()->getLine(),
                                           this->lexer->getCurrentToken()->getColumn(),
                                           payload);
            this->lexer->nextToken();
            type = RPN::RPN_OPERAND;
        }
            break;
        case Token::SYMBOL: {
            shared_ptr<OperatorToken> operatorToken = dynamic_pointer_cast<OperatorToken>(
                    this->lexer->getCurrentToken());
            switch (operatorToken->getOperatorType()) {
                case OPERATORS::SEMICOLON:
                    node = shared_ptr<Node>(nullptr);
                    type = RPN::RPN_END;
                    break;
                case OPERATORS::ROUND_BRACE_OPEN:
                    node = shared_ptr<Node>(nullptr);
                    type = RPN::RPN_ROUND_BRACE_OPEN;
                    break;
                case OPERATORS::ROUND_BRACE_CLOSE:
                    node = shared_ptr<Node>(nullptr);
                    type = RPN::RPN_ROUND_BRACE_CLOSE;
                    break;
                case OPERATORS::ADD:
                    node = make_shared<Node>(operatorToken->getLine(), operatorToken->getColumn(), Node::ADD);
                    type = RPN::RPN_ADD;
                    break;
                case OPERATORS::SUBTRACT:
                    node = make_shared<Node>(operatorToken->getLine(), operatorToken->getColumn(), Node::SUBTRACT);
                    type = RPN::RPN_SUBTRACT;
                    break;
                case OPERATORS::MULTIPLY:
                    node = make_shared<Node>(operatorToken->getLine(), operatorToken->getColumn(), Node::MULTIPLY);
                    type = RPN::RPN_MULTIPLY;
                    break;
                case OPERATORS::DIVIDE:
                    node = make_shared<Node>(operatorToken->getLine(), operatorToken->getColumn(), Node::DIVIDE);
                    type = RPN::RPN_DIVIDE;
                    break;
                case OPERATORS::POWER:
                    node = make_shared<Node>(operatorToken->getLine(), operatorToken->getColumn(), Node::POWER);
                    type = RPN::RPN_POWER;
                    break;
                case OPERATORS::LESS:
                    node = make_shared<Node>(operatorToken->getLine(), operatorToken->getColumn(), Node::LESS);
                    type = RPN::RPN_LESS;
                    break;
                case OPERATORS::GREATER:
                    node = make_shared<Node>(operatorToken->getLine(), operatorToken->getColumn(), Node::GREATER);
                    type = RPN::RPN_GREATER;
                    break;
                case OPERATORS::LESS_EQUAL:
                    node = make_shared<Node>(operatorToken->getLine(), operatorToken->getColumn(), Node::LESS_EQUAL);
                    type = RPN::RPN_LESS_EQUAL;
                    break;
                case OPERATORS::GREATER_EQUAL:
                    node = make_shared<Node>(operatorToken->getLine(), operatorToken->getColumn(), Node::GREATER_EQUAL);
                    type = RPN::RPN_GREATER_EQUAL;
                    break;
                case OPERATORS::AND:
                    node = make_shared<Node>(operatorToken->getLine(), operatorToken->getColumn(), Node::AND);
                    type = RPN::RPN_AND;
                    break;
                case OPERATORS::OR:
                    node = make_shared<Node>(operatorToken->getLine(), operatorToken->getColumn(), Node::OR);
                    type = RPN::RPN_OR;
                    break;
                case OPERATORS::EQUAL:
                    node = make_shared<Node>(operatorToken->getLine(), operatorToken->getColumn(), Node::EQUAL);
                    type = RPN::RPN_EQUAL;
                    break;
                case OPERATORS::NOTEQUAL:
                    node = make_shared<Node>(operatorToken->getLine(), operatorToken->getColumn(), Node::NOT_EQUAL);
                    type = RPN::RPN_NOTEQUAL;
                    break;
                default:
                    throw UnexpectedTokenException(this->lexer->getCurrentToken(), "term");
            }
            this->lexer->nextToken();
        }
            break;
    }

    return pair(type, node);
}
