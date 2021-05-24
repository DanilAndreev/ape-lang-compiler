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

#include "Node.h"

#include <memory>

Node::Node(const Node::TYPE type, shared_ptr<Node> op1, shared_ptr<Node> op2, shared_ptr<Node> op3) {
    this->type = type;
    this->operand1 = op1;
    this->operand2 = op2;
    this->operand3 = op3;
}

Node::~Node() {
    this->operand1 = nullptr;
    this->operand2 = nullptr;
    this->operand3 = nullptr;
}

Node::Node(const Node& reference): Node(reference.type) {
    if (reference.operand1) this->operand1 = make_shared<Node>(*reference.operand1);
    if (reference.operand2) this->operand2 = make_shared<Node>(*reference.operand2);
    if (reference.operand3) this->operand3 = make_shared<Node>(*reference.operand3);
}

void Node::destructTree() {
    if (this->operand1) {
        this->operand1->destructTree();
        this->operand1 = nullptr;
    }
    if (this->operand2) {
        this->operand2->destructTree();
        this->operand2 = nullptr;
    }
    if (this->operand3) {
        this->operand3->destructTree();
        this->operand3 = nullptr;
    }
}

Node::TYPE Node::getType() const {
    return this->type;
}

const shared_ptr<Node> Node::getOperand1() const {
    return this->operand1;
}

const shared_ptr<Node> Node::getOperand2() const {
    return this->operand2;
}

const shared_ptr<Node> Node::getOperand3() const {
    return this->operand3;
}

Node *Node::setOperand1(const shared_ptr<Node> operand) {
    this->operand1 = operand;
    return this;
}

Node *Node::setOperand2(const shared_ptr<Node>operand) {
    this->operand2 = operand;
    return this;
}

Node *Node::setOperand3(const shared_ptr<Node> operand) {
    this->operand3 = operand;
    return this;
}

ostream &Node::print(ostream &stream, int shift, string message) const {
    string shiftString(shift, ' ');
    shiftString += (char) 192;
    shiftString += (char) 196;
    stream << shiftString;
    if (message.length())
        stream << "[" << message << "] ";
    stream << this->getNodeTypeStr() << ": " << *this << endl;
    if (this->operand1) this->operand1->print(stream, shift + 2, "op1");
    if (this->operand2) this->operand2->print(stream, shift + 2, "op2");
    if (this->operand3) this->operand3->print(stream, shift + 2, "op3");
    return stream;
}

ostream &operator<<(ostream &stream, const Node &node) {
    return node.printNode(stream);
}

Node *Node::setType(Node::TYPE newType) {
    this->type = newType;
    return this;
}

string Node::getNodeTypeStr() const {
    switch (this->type) {
        case Node::PROGRAM:
            return "PROGRAM";
        case Node::EMPTY:
            return "EMPTY";
        case Node::SEQUENCE:
            return "SEQUENCE";
        case Node::SCOPE:
            return "SCOPE";
        case Node::EXPRESSION:
            return "EXPRESSION";
        case Node::VAR:
            return "VAR";
        case Node::CONST:
            return "CONST";
        case Node::SET:
            return "SET";
        case Node::IF:
            return "IF";
        case Node::IFELSE:
            return "IFELSE";
        case Node::WHILE:
            return "WHILE";
        case Node::FOR:
            return "FOR";
        case Node::DO:
            return "DO";
        case Node::ADD:
            return "ADD";
        case Node::SUBTRACT:
            return "SUBTRACT";
        case Node::MULTIPLY:
            return "MULTIPLY";
        case Node::DIVIDE:
            return "DIVIDE";
        case Node::POWER:
            return "POWER";
        case Node::LESS:
            return "LESS";
        case Node::GREATER:
            return "GREATER";
        case Node::LESS_EQUAL:
            return "LESS_EQUAL";
        case Node::GREATER_EQUAL:
            return "GREATER_EQUAL";
        case Node::EQUAL:
            return "EQUAL";
        case Node::NOT_EQUAL:
            return "NOT_EQUAL";
        case Node::PRINT:
            return "PRINT";
        case Node::READ:
            return "READ";
        case Node::AND:
            return "AND";
        case Node::OR:
            return "OR";
        case Node::CONVERT:
            return "CONVERT";
        default:
            return "UNKNOWN";
    }
}

ostream &Node::printNode(ostream &stream) const {
    return stream << "Node";
}
