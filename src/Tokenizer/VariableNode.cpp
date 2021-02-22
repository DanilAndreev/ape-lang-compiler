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

#include "VariableNode.h"

VariableNode::VariableNode(string &identifier, bool isFunc, Node* operand1): Node(Node::VAR, operand1) {
    this->identifier = identifier;
    this->isFunc = isFunc;
}

VariableNode::VariableNode(string &identifier, Node *operand1): VariableNode(identifier, false, operand1) {
    if (operand1) this->isFunc = true;
}

VariableNode::VariableNode(const VariableNode &reference): Node(reference) {
    this->identifier = reference.identifier;
    this->isFunc = reference.isFunc;
}

VariableNode::~VariableNode() {}

bool VariableNode::isFunction() const {
    return this->isFunc;
}

string VariableNode::getIdentifier() const {
    return this->identifier;
}

ostream &VariableNode::printNode(ostream &stream) const {
    stream << "VariableNode ";
    if (this->isFunc) stream << "(Func) ";
    stream << "| " << this->identifier;
    return stream;
}

VariableNode *VariableNode::setIsFunction(bool isFunc) {
    this->isFunc = isFunc;
    return this;
}
