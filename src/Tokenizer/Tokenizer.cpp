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

Tokenizer::~Tokenizer() {}

shared_ptr<Node> Tokenizer::parse() {
    this->lexer->nextToken();
    shared_ptr<Node> node = make_shared<Node>(Node::PROGRAM, this->statement());
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
                case KEYWORDS::FOR: {
                    shared_ptr<Node> forNode = make_shared<Node>(Node::FOR);
                    node = make_shared<Node>(Node::SEQUENCE);
                    node->setOperand2(forNode);

                    this->lexer->nextToken();
                    if (this->lexer->getCurrentToken()->getType() != Token::SYMBOL)
                        throw ApeCompilerException("Expected \"(\"");
                    if (dynamic_pointer_cast<OperatorToken>(this->lexer->getCurrentToken())->getOperatorType() !=
                        OPERATORS::ROUND_BRACE_OPEN)
                        throw ApeCompilerException("Expected \"(\"");

                    if (this->lexer->nextToken()->getType() == Token::KEYWORD) {
                        node->setOperand1(this->declaration());
                    } else {
                        node->setOperand1(this->expression());
                    }

                    if (this->lexer->getCurrentToken()->getType() != Token::SYMBOL)
                        throw ApeCompilerException("Expected \";\"");
                    if (dynamic_pointer_cast<OperatorToken>(this->lexer->getCurrentToken())->getOperatorType() !=
                        OPERATORS::SEMICOLON)
                        throw ApeCompilerException("Expected \";\"");


                    this->lexer->nextToken();
                    forNode->setOperand1(this->test());

                    if (this->lexer->getCurrentToken()->getType() != Token::SYMBOL)
                        throw ApeCompilerException("Expected \";\"");
                    if (dynamic_pointer_cast<OperatorToken>(this->lexer->getCurrentToken())->getOperatorType() !=
                        OPERATORS::SEMICOLON)
                        throw ApeCompilerException("Expected \";\"");

                    this->lexer->nextToken();
                    forNode->setOperand2(this->expression());

                    if (this->lexer->getCurrentToken()->getType() != Token::SYMBOL)
                        throw ApeCompilerException("Expected \")\"");
                    if (dynamic_pointer_cast<OperatorToken>(this->lexer->getCurrentToken())->getOperatorType() !=
                        OPERATORS::ROUND_BRACE_CLOSE)
                        throw ApeCompilerException("Expected \")\"");

                    this->lexer->nextToken();
                    forNode->setOperand3(this->statement());
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
                    dynamic_pointer_cast<VariableNode>(node)->setConstant(true);
                }
                    break;
                case KEYWORDS::PRINT: {
                    this->lexer->nextToken();
                    if (this->lexer->getCurrentToken()->getType() != Token::SYMBOL)
                        throw ApeCompilerException("Expected \"(\"");
                    if (dynamic_pointer_cast<OperatorToken>(this->lexer->getCurrentToken())->getOperatorType() !=
                        OPERATORS::ROUND_BRACE_OPEN)
                        throw ApeCompilerException("Expected \"(\"");

                    this->lexer->nextToken();
                    node = this->test();

                    if (this->lexer->getCurrentToken()->getType() != Token::SYMBOL)
                        throw ApeCompilerException("Expected \")\"");
                    if (dynamic_pointer_cast<OperatorToken>(this->lexer->getCurrentToken())->getOperatorType() !=
                        OPERATORS::ROUND_BRACE_CLOSE)
                        throw ApeCompilerException("Expected \")\"");

                    this->lexer->nextToken();
                    if (this->lexer->getCurrentToken()->getType() != Token::SYMBOL)
                        throw ApeCompilerException("Expected \";\"");
                    if (dynamic_pointer_cast<OperatorToken>(this->lexer->getCurrentToken())->getOperatorType() !=
                        OPERATORS::SEMICOLON)
                        throw ApeCompilerException("Expected \";\"");
                    node = make_shared<Node>(Node::PRINT, node);
                }
                    break;
                case KEYWORDS::READ: {
                    this->lexer->nextToken();
                    if (this->lexer->getCurrentToken()->getType() != Token::SYMBOL)
                        throw ApeCompilerException("Expected \"(\"");
                    if (dynamic_pointer_cast<OperatorToken>(this->lexer->getCurrentToken())->getOperatorType() !=
                        OPERATORS::ROUND_BRACE_OPEN)
                        throw ApeCompilerException("Expected \"(\"");

                    this->lexer->nextToken();
                    if (this->lexer->getCurrentToken()->getType() != Token::IDENTIFIER)
                        throw ApeCompilerException("Expected identifier");

                    string varName = this->lexer->getCurrentToken()->getPayload();
                    node = make_shared<VariableNode>(varName, false);

                    this->lexer->nextToken();
                    if (this->lexer->getCurrentToken()->getType() != Token::SYMBOL)
                        throw ApeCompilerException("Expected \")\"");
                    if (dynamic_pointer_cast<OperatorToken>(this->lexer->getCurrentToken())->getOperatorType() !=
                        OPERATORS::ROUND_BRACE_CLOSE)
                        throw ApeCompilerException("Expected \")\"");

                    this->lexer->nextToken();
                    if (this->lexer->getCurrentToken()->getType() != Token::SYMBOL)
                        throw ApeCompilerException("Expected \";\"");
                    if (dynamic_pointer_cast<OperatorToken>(this->lexer->getCurrentToken())->getOperatorType() !=
                        OPERATORS::SEMICOLON)
                        throw ApeCompilerException("Expected \";\"");
                    node = make_shared<Node>(Node::READ, node);
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
                    node = make_shared<Node>(Node::SCOPE, node);
                    this->lexer->nextToken();
                }
                    break;
            }
        }
            break;
        default:
            node = make_shared<Node>(Node::EXPRESSION, this->expression());
            if (this->lexer->getCurrentToken()->getType() != Token::SYMBOL)
                throw ApeCompilerException("Expected \";\"");
            shared_ptr<OperatorToken> token = dynamic_pointer_cast<OperatorToken>(this->lexer->getCurrentToken());
            if (token->getOperatorType() != OPERATORS::SEMICOLON)
                throw ApeCompilerException("Expected \";\"");
            this->lexer->nextToken();
    }

    return node;
}

shared_ptr<Node> Tokenizer::parenExpr() const {
    shared_ptr<Node> node = nullptr;

    shared_ptr<Token> token = this->lexer->getCurrentToken();
    if (token->getType() != Token::SYMBOL)
        throw ApeCompilerException("Expected \"(\", got token type: \'" + token->getType());
    shared_ptr<OperatorToken> keyword = dynamic_pointer_cast<OperatorToken>(token);
    if (keyword->getOperatorType() != OPERATORS::ROUND_BRACE_OPEN)
        throw ApeCompilerException("Expected \"(\", got \"" + keyword->getPayload() + "\n");

    this->lexer->nextToken();
    node = this->expression();

    token = this->lexer->getCurrentToken();
    if (token->getType() != Token::SYMBOL)
        throw ApeCompilerException("Expected \")\"");
    keyword = dynamic_pointer_cast<OperatorToken>(token);
    if (keyword->getOperatorType() != OPERATORS::ROUND_BRACE_CLOSE)
        throw ApeCompilerException("Expected \")\"");

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

shared_ptr<VariableNode> Tokenizer::declaration(bool initialization, bool semicolon) const {
    shared_ptr<VariableNode> node = nullptr;
    shared_ptr<Token> token = this->lexer->getCurrentToken();
    if (token->getType() != Token::KEYWORD)
        throw ApeCompilerException("Incorrect type");

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
    }
    shared_ptr<Token> current = this->lexer->nextToken();
    if (current->getType() != Token::IDENTIFIER)
        throw ApeCompilerException("Incorrect daclaration");
    string name = current->getPayload();
    node = make_shared<VariableNode>(name, false);
    node->setBasicType(dataType);
    node->setDeclaration(true);
    current = this->lexer->nextToken();
    if (initialization && current->getType() == Token::SYMBOL) {
        shared_ptr<OperatorToken> opToken = dynamic_pointer_cast<OperatorToken>(current);
        if (opToken->getOperatorType() == OPERATORS::ASSIGN) {
            this->lexer->nextToken();
            node->setOperand1(this->test());
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

    shared_ptr<Node> node = make_shared<Node>(Node::EMPTY);

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
        throw ApeCompilerException("Expected \"(\", got not symbol token");
    shared_ptr<OperatorToken> opToken = dynamic_pointer_cast<OperatorToken>(token);
    if (opToken == nullptr || opToken->getOperatorType() != OPERATORS::ROUND_BRACE_OPEN)
        throw ApeCompilerException("Expected \"(\" got \"" + opToken->getPayload() + "\"");
    token = this->lexer->nextToken();

    shared_ptr<Node> node = make_shared<Node>(Node::EMPTY);

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

pair<shared_ptr<Node>, shared_ptr<vector<ApeCompilerException>>> Tokenizer::validateTree(
        const shared_ptr<Node> input,
        shared_ptr<Scope> scope,
        shared_ptr<Scope> outerScope,
        shared_ptr<vector<ApeCompilerException>> errors
) {
    if (scope == nullptr)
        scope = make_shared<Scope>();
    if (outerScope == nullptr)
        outerScope = make_shared<Scope>();
    if (errors == nullptr)
        errors = make_shared<vector<ApeCompilerException>>();

    const shared_ptr<Node> operand1 = input->getOperand1();
    const shared_ptr<Node> operand2 = input->getOperand2();
    const shared_ptr<Node> operand3 = input->getOperand3();

    switch (input->getType()) {
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
                if (scope->find(variable->getIdentifier()) != scope->end()) {
                    errors->push_back(ApeCompilerException("Re-declaration of variable " + variable->getIdentifier()));
                } else {
                    variable->setIndex();
                    scope->insert(ScopeItem(variable->getIdentifier(), variable));
                }
            } else {
                auto declaration = scope->find(variable->getIdentifier());
                auto outerDeclaration = outerScope->find(variable->getIdentifier());
                if (declaration != scope->end()) {
                    variable->setBasicType(declaration->second->getBasicType());
                    variable->setIndex(declaration->second->getIndex());
                } else if (outerDeclaration != outerScope->end()) {
                    variable->setBasicType(outerDeclaration->second->getBasicType());
                    variable->setIndex(outerDeclaration->second->getIndex());
                } else {
                    errors->push_back(ApeCompilerException("Undeclared variable " + variable->getIdentifier()));
                }
            }
        }
            break;
        default:
            if (operand1 != nullptr) validateTree(operand1, scope, outerScope, errors);
            if (operand2 != nullptr) validateTree(operand2, scope, outerScope, errors);
            if (operand3 != nullptr) validateTree(operand3, scope, outerScope, errors);

    }

    return pair<shared_ptr<Node>, shared_ptr<vector<ApeCompilerException>>>(input, errors);

/*
    if (scope == nullptr)
        scope = make_shared<Scope>();
    if (outerScope == nullptr)
        outerScope = make_shared<Scope>();
    if (errors == nullptr)
        errors = make_shared<vector<ApeCompilerException>>();

    shared_ptr<Scope> inputScope = make_shared<Scope>(*scope);

    const shared_ptr<Node> operand1 = input->getOperand1();
    const shared_ptr<Node> operand2 = input->getOperand2();
    const shared_ptr<Node> operand3 = input->getOperand3();

    switch (input->getType()) {
        case Node::SEQUENCE:
            validateTree(operand1, scope, outerScope, errors);
            validateTree(operand2, scope, outerScope, errors);
            break;
        case Node::SET: {
            // TODO: dismiss expression: int a = a + 1;
            shared_ptr<VariableNode> variable = dynamic_pointer_cast<VariableNode>(operand1);
            if (variable == nullptr) {
                errors->push_back(ApeCompilerException("Incorrect variable node on set operation"));
            }
            string identifier = variable->getIdentifier();
            auto declaration = scope->find(identifier); // TODO: search in both scopes
            if (declaration != scope->end()) {
                if (declaration->second->isConstant()) {
                    errors->push_back(ApeCompilerException("Assigning to const variable " + identifier));
                }
                if (declaration->second->isFunction()) {
                    errors->push_back(ApeCompilerException("Assigning to function " + identifier));
                }
            }
            validateTree(operand1, scope, outerScope, errors);
            validateTree(operand2, scope, outerScope, errors);
        }
            break;
        case Node::VAR: {
            shared_ptr<VariableNode> declaration = dynamic_pointer_cast<VariableNode>(input);
            if (declaration != nullptr) {
                if (scope->find(declaration->getIdentifier()) != scope->end()) {
                    //throw ApeCompilerException("Re-declaration of variable " + declaration->getIdentifier());
                    errors->push_back(
                            ApeCompilerException("Re-declaration of variable " + declaration->getIdentifier()));
                }
                scope->insert(ScopeItem(declaration->getIdentifier(), declaration));

                if (declaration->isFunction()) {
                    // TODO: check function args
//                    shared_ptr<Scope> innerOuterScope = make_shared<Scope>(*inputScope);
//                    innerOuterScope->insert(outerScope->begin(), outerScope->end());
                    validateTree(operand2, scope, outerScope, errors);
                } else {
                    if (operand1 != nullptr)
                        validateTree(operand1, inputScope, outerScope, errors);
                }
            } else {
                shared_ptr<VariableNode> variable = dynamic_pointer_cast<VariableNode>(input);
                if (variable == nullptr)
                    throw ApeCompilerException("Fatal token error");
                if (
                        scope->find(variable->getIdentifier()) == scope->end() &&
                        outerScope->find(variable->getIdentifier()) == outerScope->end()
                        ) {
                    // throw ApeCompilerException("Undeclared variable " + variable->getIdentifier());
                    errors->push_back(ApeCompilerException("Undeclared variable " + variable->getIdentifier()));
                }


                // TODO: if func - check arguments type correctness
            }
        }
            break;
        case Node::SCOPE: {
            shared_ptr<Scope> newOuterScope = make_shared<Scope>(*inputScope);
            newOuterScope->insert(outerScope->begin(), outerScope->end());
            if (operand1 != nullptr) validateTree(operand1, nullptr, newOuterScope, errors);
            if (operand2 != nullptr) validateTree(operand2, nullptr, newOuterScope, errors);
            if (operand3 != nullptr) validateTree(operand3, nullptr, newOuterScope, errors);
        }
            break;
        default:
            if (operand1 != nullptr) validateTree(operand1, scope, outerScope, errors);
            if (operand2 != nullptr) validateTree(operand2, scope, outerScope, errors);
            if (operand3 != nullptr) validateTree(operand3, scope, outerScope, errors);
    }

    return pair<shared_ptr<Node>, shared_ptr<vector<ApeCompilerException>>>(input, errors);
*/
}
