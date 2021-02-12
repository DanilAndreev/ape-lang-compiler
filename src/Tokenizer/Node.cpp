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

Node::Node(Node::TYPE type, Node *op1 = nullptr, Node *op2 = nullptr, Node *op3 = nullptr) {
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
