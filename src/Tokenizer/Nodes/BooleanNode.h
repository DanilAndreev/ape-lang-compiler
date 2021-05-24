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

#ifndef APE_LANG_COMPILER_BOOLEANNODE_H
#define APE_LANG_COMPILER_BOOLEANNODE_H


#include "LiteralNode.h"

/**
 * BoolNode - class for storing boolean literal nodes.
 * @author Danil Andreev
 */
class BooleanNode : public LiteralNode {
protected:
    /// payload of boolean literal.
    bool payload;
public:
    explicit BooleanNode(int line, int column, bool payload);

    BooleanNode(const BooleanNode &reference);

    ~BooleanNode() override;

public:
    /**
     * getPayload - getter for floating point literal payload.
     * @author Danil Andreev
     */
    bool getPayload() const;

public:
    std::ostream &printNode(std::ostream &stream) const override;
};


#endif //APE_LANG_COMPILER_BOOLEANNODE_H
