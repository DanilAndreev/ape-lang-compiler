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
        IF,
        WHILE,
        FOR,

        ADD,
        SUB,
    };
protected:
    /**
     * type - node type.
     */
    TYPE type;
    /**
     * operand1 - first nested operand of the node.
     */
    Node *operand1;
    /**
     * operand2 - second nested operand of the node.
     */
    Node *operand2;
    /**
     * operand3 - first nested operand of the node.
     */
    Node *operand3;
public:
    Node(TYPE type, Node *operand1, Node *operand2, Node *operand3);

    ~Node();

    /**
     * destructTree - calls destructTree methods for all nested operands and deletes them.
     * After deleting operand will be assigned nullptr.
     * @author Danil Andreev
     */
    void destructTree();

    /**
     * getType - getter for node type.
     */
    TYPE getType() const;

    /**
     * getOperand1 - getter for node first operand.
     */
    Node const *getOperand1() const;

    /**
     * getOperand2 - getter for node second operand.
     */
    Node const *getOperand2() const;

    /**
     * getOperand3 - getter for node third operand.
     */
    Node const *getOperand3() const;
};


#endif //APE_LANG_COMPILER_NODE_H
