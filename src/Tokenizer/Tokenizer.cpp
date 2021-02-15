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

Tokenizer::Tokenizer(Lexer *lexer) {
    this->lexer = lexer;
}

Tokenizer::Tokenizer(Lexer &lexer) : Tokenizer(&lexer) {}

Tokenizer::Tokenizer(const Tokenizer &reference) {
    this->lexer = reference.lexer;
}

Node *Tokenizer::parse() {
    this->lexer->nextToken();
    Node *node = new Node(Node::PROGRAM, this->statement());
    if (!this->lexer->isEof())
        throw new exception();
    return node;
}

Node *Tokenizer::statement() const {
    Token entry = this->lexer->getCurrentToken();
    Node *node = nullptr;

    /// KEYWORDS
    if (entry.getType() == Token::KEYWORD) {
        KeywordToken *token = dynamic_cast<KeywordToken*>(&entry);

        /// IF
        if (token->getKeywordType() == KEYWORDS::IF) {
            node = new Node(Node::IF);
            this->lexer->nextToken();
            node->setOperand1(this->parenExpr());
            node->setOperand2(this->statement());
            if (this->lexer->getCurrentToken().getType() == Token::KEYWORD) {
                entry = this->lexer->getCurrentToken();
                KeywordToken* elseToken = dynamic_cast<KeywordToken*>(&entry);
                /// ELSE
                if (elseToken->getKeywordType() == KEYWORDS::ELSE) {
                    node->setType(Node::IFELSE);
                    this->lexer->nextToken();
                    node->setOperand3(this->statement());
                }
            }
        }


    }

    return node;
}

Node *Tokenizer::parenExpr() const {
    Node *node = nullptr;

    Token token = this->lexer->getCurrentToken();
    if (token.getType() != Token::SYMBOL)
        throw exception();
    OperatorToken *keyword = dynamic_cast<OperatorToken *>(&token);
    if (keyword->getOperatorType() != OPERATORS::ROUND_BRACE_OPEN)
        throw exception();

    this->lexer->nextToken();
    node = this->expression();

    token = this->lexer->getCurrentToken();
    if (token.getType() != Token::SYMBOL)
        throw exception();
    keyword = dynamic_cast<OperatorToken *>(&token);
    if (keyword->getOperatorType() != OPERATORS::ROUND_BRACE_CLOSE)
        throw exception();

    this->lexer->nextToken();
    return node;
}

Node *Tokenizer::expression() const {
    Node *node = this->test();

    if (this->lexer->getCurrentToken().getType() == Token::IDENTIFIER)
        return node;

//    if () //TODO: finish

    return node;
}

Node *Tokenizer::test() const {
    Node *node = this->summa();

    Token token = this->lexer->getCurrentToken();
    if (token.getType() == Token::SYMBOL) {
        OperatorToken *operatorToken = dynamic_cast<OperatorToken *>(&token);
        if (operatorToken->getOperatorType() == OPERATORS::LESS) {
            this->lexer->nextToken();
            node = new Node(Node::LESS, node, this->summa());
        }
    }

    return node;
}

Node *Tokenizer::summa() const {
    Node *node = this->term();

    // TODO: finish summa;

    return node;
}

Node *Tokenizer::term() const {
    Node *node = nullptr;
    if (this->lexer->getCurrentToken().getType() == Token::IDENTIFIER) {
        node = new Node(Node::VAR);
        //TODO: finish var nodes
        // node.value = this->lexer->getCurrentToken().getPayload();
        this->lexer->nextToken();
//        return node;
    } else if (this->lexer->getCurrentToken().getType() == Token::NUMBER) {
        node = new Node(Node::CONST);
        //TODO: finish var nodes
        // node.value = this->lexer->getCurrentToken().getPayload();
//        return node;
    } else {
        return this->parenExpr();
    }

    return node;
}
