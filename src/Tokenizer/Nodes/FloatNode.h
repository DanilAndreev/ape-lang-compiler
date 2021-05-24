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

#ifndef APE_LANG_COMPILER_FLOATNODE_H
#define APE_LANG_COMPILER_FLOATNODE_H


#include "LiteralNode.h"

/**
 * FloatNode - class for storing floating point number literal nodes.
 * @author Danil Andreev
 */
class FloatNode: public LiteralNode {
protected:
    /// payload of floating point number literal.
    long double payload;
public:
    explicit FloatNode(long double payload);
    FloatNode(const FloatNode& reference);
    ~FloatNode() override;
public:
    /**
     * getPayload - getter for floating point literal payload.
     * @author Danil Andreev
     */
    long double getPayload() const;
public:
    std::ostream& printNode(std::ostream& stream) const override;
};


#endif //APE_LANG_COMPILER_FLOATNODE_H
