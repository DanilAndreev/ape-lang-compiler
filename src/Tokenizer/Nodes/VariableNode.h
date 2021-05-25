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
 * ConvertNode - AST node for storing variable information.
 * @class
 * @author Danil Andreev
 */
class VariableNode : public Node {
protected:
    static unsigned int index_counter;
public:
    /**
     * DATA_TYPE - enumeration for variable data types.
     * @enum
     * @author Danil Andreev
     */
    enum DATA_TYPE {
        INT,
        FLOAT,
        BOOLEAN,
        STRING,
        VOID,
    };

public:
    /**
     * nextIndex - method for generating next available unique variable id.
     * @return Next available unique id.
     * @author Danil Andreev.
     */
    static unsigned int nextIndex();

    /**
     * getDataTypeStr - converts enum value of data type to string representing it for printing.
     * @param input - input data type.
     * @return String for printing.
     * @author Danil Andreev
     */
    static std::string getDataTypeStr(DATA_TYPE input);

protected:
    /// identifier - variable identifier string
    std::string identifier;
    /// isFunc - if true, this variable refers to function.
    bool isFunc;
    /// dataType - variable data type.
    DATA_TYPE dataType;
    /// dataType - variable constant state. If true - variable is a constant.
    bool constant;
    /// dataType - variable declaration state. If true - node represents a variable declaration.
    bool declaration;
    /// dataType - variable global index.
    unsigned int index;
public:
    explicit VariableNode(int line, int column, std::string &identifier, bool isFunc = false,
                          std::shared_ptr<Node> operand1 = nullptr);

    explicit VariableNode(int line, int column, std::string &identifier, std::shared_ptr<Node> operand1 = nullptr);

    VariableNode(const VariableNode &reference);

    ~VariableNode() override;

public:
    /**
     * getIdentifier - getter for variable identifier string.
     * @author Danil Andreev
     */
    std::string getIdentifier() const;

    /**
     * getIndex - getter for global variable index.
     * @return Variable index.
     * @author Danil Andreev
     */
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

    /**
     * setConstant - sets constant state to variable.
     * @author Danil Andreev
     */
    VariableNode *setConstant(bool value = true);

    /**
     * setConstant - sets declaration state to variable.
     * @author Danil Andreev
     */
    VariableNode *setDeclaration(bool value = true);

    /**
     * setConstant - sets variable type.
     * @author Danil Andreev
     */
    VariableNode *setBasicType(DATA_TYPE value);

    /**
     * setConstant - sets variable global index.
     * @author Danil Andreev
     */
    VariableNode *setIndex(unsigned int index);

    /**
     * setConstant - sets next free global variable index.
     * @author Danil Andreev
     */
    VariableNode *setIndex();

public:
    /**
     * setIsFunction - setter for isFunc field.
     * @author Danil Andreev
     */
    VariableNode *setIsFunction(bool isFunc);

public:
    std::ostream &printNode(std::ostream &stream) const override;
};


#endif //APE_LANG_COMPILER_VARIABLENODE_H
