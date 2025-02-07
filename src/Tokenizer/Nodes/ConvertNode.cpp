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

#include "ConvertNode.h"

using namespace std;

ConvertNode::ConvertNode(
        const int line,
        const int column,
        const VariableNode::DATA_TYPE dateType,
        shared_ptr<Node> operand1
) : Node(line, column, Node::CONVERT, operand1) {
    this->dataType = dateType;
}

ConvertNode::ConvertNode(const ConvertNode &reference) : Node(reference) {
    this->dataType = reference.dataType;
}

ConvertNode::~ConvertNode() {
}

VariableNode::DATA_TYPE ConvertNode::getDataType() const {
    return this->dataType;
}

ConvertNode *ConvertNode::setDataType(const VariableNode::DATA_TYPE iDataType) {
    this->dataType = iDataType;
    return this;
}

ostream &ConvertNode::printNode(ostream &stream) const {
    return stream << "ConvertNode | " << VariableNode::getDataTypeStr(this->dataType);
}
