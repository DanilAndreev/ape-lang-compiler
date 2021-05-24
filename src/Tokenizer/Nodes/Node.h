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

#ifndef APE_LANG_COMPILER_NODE_H
#define APE_LANG_COMPILER_NODE_H

#include <iostream>
#include <memory>

using namespace std;

/**
 * Node - class for storing Syntax tree node.
 * @see https://en.wikipedia.org/wiki/Abstract_syntax_tree
 * @author Danil Andreev
 */
class Node {
public:
    /**
     * TYPE - Node types enumeration.
     */
    enum TYPE {
        PROGRAM,
        EMPTY,
        SEQUENCE,
        SCOPE,
        EXPRESSION,

        VAR,
        CONST,

        SET,

        IF,
        IFELSE,
        WHILE,
        FOR,
        DO,

        ADD,
        SUBTRACT,
        MULTIPLY,
        DIVIDE,
        POWER,

        LESS,
        GREATER,
        LESS_EQUAL,
        GREATER_EQUAL,
        EQUAL,
        NOT_EQUAL,

        AND,
        OR,

        PRINT,
        READ,

        CONVERT,
    };
protected:
    /**
     * type - node type.
     */
    TYPE type;
    /**
     * operand1 - first nested operand of the node.
     */
    shared_ptr<Node> operand1;
    /**
     * operand2 - second nested operand of the node.
     */
    shared_ptr<Node> operand2;
    /**
     * operand3 - first nested operand of the node.
     */
    shared_ptr<Node> operand3;
public:
    Node(
            const TYPE type,
            shared_ptr<Node> operand1 = nullptr,
            shared_ptr<Node> operand2 = nullptr,
            shared_ptr<Node> operand3 = nullptr
    );

    Node(const Node &reference);

    virtual ~Node();

public:
    /**
     * getType - getter for node type.
     * @author Danil Andreev
     */
    TYPE getType() const;

    /**
     * getOperand1 - getter for node first operand.
     * @author Danil Andreev
     */
    const shared_ptr<Node> getOperand1() const;

    /**
     * getOperand2 - getter for node second operand.
     * @author Danil Andreev
     */
    const shared_ptr<Node> getOperand2() const;

    /**
     * getOperand3 - getter for node third operand.
     * @author Danil Andreev
     */
    const shared_ptr<Node> getOperand3() const;

public:
    /**
     * setType - setter for node type.
     * @param type - New node type.
     * @author Danil Andreev
     */
    Node *setType(Node::TYPE type);

    /**
     * setOperand1 - setter for node first operand.
     * @paran operand - Target node for nesting.
     * @author Danil Andreev
     */
    Node *setOperand1(const shared_ptr<Node> operand);

    /**
     * setOperand2 - setter for node second operand.
     * @paran operand - Target node for nesting.
     * @author Danil Andreev
     */
    Node *setOperand2(const shared_ptr<Node> operand);

    /**
     * setOperand3 - setter for node third operand.
     * @paran operand - Target node for nesting.
     * @author Danil Andreev
     */
    Node *setOperand3(const shared_ptr<Node> operand);

public:
    virtual string getNodeTypeStr() const;

    /**
     * destructTree - calls destructTree methods for all nested operands and deletes them.
     * After deleting operand will be assigned nullptr.
     * @author Danil Andreev
     */
    void destructTree();

    /**
     * print - method designed for printing tree structure to stream.
     * @param stream - stream object for printing.
     */
    ostream &print(ostream &stream = cout, int shift = 0, string message = "") const;

    virtual ostream &printNode(ostream &stream) const;

public:
    friend ostream &operator<<(ostream &stream, const Node &node);
};


#endif //APE_LANG_COMPILER_NODE_H
