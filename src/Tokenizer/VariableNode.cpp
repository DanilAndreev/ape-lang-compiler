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

unsigned int VariableNode::index_counter = 1;

VariableNode::VariableNode(string &identifier, bool isFunc, shared_ptr<Node> operand1): Node(Node::VAR, operand1) {
    this->identifier = identifier;
    this->isFunc = isFunc;
    this->declaration = false;
    this->index = 0;
    this->dataType = DATA_TYPE::INT;
    this->constant = false;
}

VariableNode::VariableNode(string &identifier,  shared_ptr<Node> operand1): VariableNode(identifier, false, operand1) {
    if (operand1) this->isFunc = true;
    this->declaration = false;
    this->index = 0;
    this->dataType = DATA_TYPE::INT;
    this->constant = false;
}

VariableNode::VariableNode(const VariableNode &reference): Node(reference) {
    this->identifier = reference.identifier;
    this->isFunc = reference.isFunc;
    this->declaration = reference.declaration;
    this->index = reference.index;
    this->dataType = reference.dataType;
    this->constant = reference.constant;
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
    if (this->declaration) stream << "Declaration ";
    if (this->index) stream << "Index(" << this->index << ") ";
    stream << "| " << this->identifier;
    return stream;
}

VariableNode *VariableNode::setIsFunction(bool isFunc) {
    this->isFunc = isFunc;
    return this;
}

bool VariableNode::isConstant() const {
    return this->constant;
}

VariableNode::DATA_TYPE VariableNode::getBasicType() const {
    return this->dataType;
}

bool VariableNode::isDeclaration() {
    return this->declaration;
}

VariableNode *VariableNode::setConstant(bool value) {
    this->constant = value;
    return this;
}

VariableNode *VariableNode::setDeclaration(bool value) {
    this->declaration = value;
    return this;
}

VariableNode *VariableNode::setBasicType(VariableNode::DATA_TYPE type) {
    this->dataType = type;
    return this;
}

unsigned int VariableNode::nextIndex() {
    return index_counter++;
}

VariableNode *VariableNode::setIndex(unsigned int index) {
    this->index = index;
    return this;
}

VariableNode *VariableNode::setIndex() {
    this->index = nextIndex();
    return this;
}

unsigned int VariableNode::getIndex() const {
    return this->index;
}
