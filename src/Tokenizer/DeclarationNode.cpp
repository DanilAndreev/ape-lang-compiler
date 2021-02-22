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

#include "DeclarationNode.h"

DeclarationNode::DeclarationNode(DeclarationNode::DATA_TYPE dataType, string &name, bool isFunc, Node *operand1)
        : VariableNode(name, isFunc, operand1) {
    this->dataType = dataType;
    this->constant = false;
}

DeclarationNode::DeclarationNode(const DeclarationNode &reference) : VariableNode(reference) {
    this->constant = reference.constant;
    this->dataType = reference.dataType;
}

DeclarationNode::~DeclarationNode() {}

DeclarationNode::DATA_TYPE DeclarationNode::getBasicType() const {
    return this->dataType;
}

DeclarationNode *DeclarationNode::setConstant(bool constant) {
    this->constant = constant;
    return this;
}

bool DeclarationNode::isConstant() const {
    return this->constant;
}

Node *DeclarationNode::setDataType(DeclarationNode::DATA_TYPE dataType) {
    this->dataType = dataType;
    return this;
}

ostream &DeclarationNode::printNode(ostream &stream) const {
    stream << "DeclarationNode | ";
    if (this->constant) stream << "const ";
    stream << this->getDatTypeString() << " | " << this->identifier;
    return stream;
}

string DeclarationNode::getDatTypeString() const {
    switch (this->dataType) {
        case INT:
            return "INT";
        case BOOLEAN:
            return "BOOLEAN";
        case FLOAT:
            return "FLOAT";
        case STRING:
            return "STRING";
    }
    return "UNKNOWN";
}
