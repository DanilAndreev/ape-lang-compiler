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

Node::Node(Node::TYPE type, Node *op1, Node *op2, Node *op3) {
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

void Node::destructTree() {
    if (this->operand1) {
        this->operand1->destructTree();
        delete this->operand1;
        this->operand1 = nullptr;
    }
    if (this->operand2) {
        this->operand2->destructTree();
        delete this->operand2;
        this->operand2 = nullptr;
    }
    if (this->operand3) {
        this->operand3->destructTree();
        delete this->operand3;
        this->operand3 = nullptr;
    }
}

Node::TYPE Node::getType() const {
    return this->type;
}

Node const *Node::getOperand1() const {
    return this->operand1;
}

Node const *Node::getOperand2() const {
    return this->operand2;
}

Node const *Node::getOperand3() const {
    return this->operand3;
}

Node *Node::setOperand1(Node *operand) {
    this->operand1 = operand;
    return this;
}

Node *Node::setOperand2(Node *operand) {
    this->operand2 = operand;
    return this;
}

Node *Node::setOperand3(Node *operand) {
    this->operand3 = operand;
    return this;
}

ostream &Node::print(ostream &stream, int shift, string message) const {
    string shiftString(shift, ' ');
    shiftString += (char) 192;
    shiftString += (char) 196;
    stream << shiftString << this->type << ": ";
    if (message.length())
        stream << "[" << message << "] ";
    stream << *this << endl;
    if (this->operand1) this->operand1->print(stream, shift + 2, "op1");
    if (this->operand2) this->operand2->print(stream, shift + 2, "op2");
    if (this->operand3) this->operand3->print(stream, shift + 2, "op3");
    return stream;
}

ostream &operator<<(ostream &stream, const Node &node) {
    stream << "Node";
    return stream;
}

Node *Node::setType(Node::TYPE newType) {
    this->type = newType;
    return this;
}
