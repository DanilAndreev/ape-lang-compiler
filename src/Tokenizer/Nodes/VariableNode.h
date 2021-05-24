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

#ifndef APE_LANG_COMPILER_VARIABLENODE_H
#define APE_LANG_COMPILER_VARIABLENODE_H


#include <memory>
#include "Node.h"

/**
 * VariableNode - class for storing variable nodes.
 * @author Danil Andreev
 */
class VariableNode : public Node {
protected:
    static unsigned int index_counter;
public:
    enum DATA_TYPE {
        INT,
        FLOAT,
        BOOLEAN,
        STRING,
        VOID,
    };

public:
    static unsigned int nextIndex();
    static string getDataTypeStr(DATA_TYPE input);
protected:
    /// identifier - variable identifier string
    string identifier;
    /// isFunc - if true, this variable refers to function.
    bool isFunc;
    DATA_TYPE dataType;
    bool constant;
    bool declaration;
    unsigned int index;
public:
    explicit VariableNode(string &identifier, bool isFunc = false, shared_ptr<Node> operand1 = nullptr);

    explicit VariableNode(string &identifier, shared_ptr<Node> operand1 = nullptr);

    VariableNode(const VariableNode &reference);

    ~VariableNode() override;

public:
    /**
     * getIdentifier - getter for variable identifier string.
     * @author Danil Andreev
     */
    string getIdentifier() const;

    unsigned int getIndex() const;

    /**
     * isFunction - if variable refers to function return true, else false.
     * @author Danil Andreev
     */
    bool isFunction() const;

    /**
     * isFunction - if variable is marked as constant return true, else false.
     * @author Danil Andreev
     */
    bool isConstant() const;

    /**
     * isFunction - if nmode represents variable declaration return true, else false.
     * @author Danil Andreev
     */
    bool isDeclaration();

    /**
     * isFunction - getter for variable type.
     * @author Danil Andreev
     */
    DATA_TYPE getBasicType() const;

    VariableNode *setConstant(bool value = true);

    VariableNode *setDeclaration(bool value = true);

    VariableNode *setBasicType(DATA_TYPE value);

    VariableNode *setIndex(unsigned int index);

    VariableNode *setIndex();

public:
    /**
     * setIsFunction - setter for isFunc field.
     * @author Danil Andreev
     */
    VariableNode *setIsFunction(bool isFunc);

public:
    ostream &printNode(ostream &stream) const override;
};


#endif //APE_LANG_COMPILER_VARIABLENODE_H
