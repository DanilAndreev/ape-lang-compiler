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

#ifndef APE_LANG_COMPILER_CONVERTNODE_H
#define APE_LANG_COMPILER_CONVERTNODE_H


#include "Node.h"
#include "VariableNode.h"

/**
 * ConvertNode - AST node for data type conversion.
 * @class
 * @author Danil Andreev
 */
class ConvertNode : public Node {
protected:
    /// dataType - target data type after conversion.
    VariableNode::DATA_TYPE dataType;
public:
    explicit ConvertNode(int line, int column, const VariableNode::DATA_TYPE dateType,
                         std::shared_ptr<Node> operand1 = nullptr);

    ConvertNode(const ConvertNode &reference);

    ~ConvertNode() override;

public:
    /**
     * getDataType - getter for target data type.
     * @return Data type after conversion.
     * @author Danil Andreev
     */
    VariableNode::DATA_TYPE getDataType() const;

    /**
     * setDataType - sets target data type.
     * @param type - Conversion data type.
     * @author Danil Andreev
     */
    ConvertNode *setDataType(const VariableNode::DATA_TYPE type);

public:
    std::ostream &printNode(std::ostream &stream) const override;
};


#endif //APE_LANG_COMPILER_CONVERTNODE_H
