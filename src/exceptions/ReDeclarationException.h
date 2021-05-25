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

#ifndef APE_LANG_COMPILER_REDECLARATIONEXCEPTION_H
#define APE_LANG_COMPILER_REDECLARATIONEXCEPTION_H


#include "CodeException.h"
#include "../Tokenizer/Nodes/VariableNode.h"

/**
 * CodeException - exception class for variable re-declaration errors.
 * @class
 * @author Danil Andreev
 */
class ReDeclarationException : public CodeException {
public:
    ReDeclarationException(std::shared_ptr<VariableNode> problem, std::shared_ptr<Positionable> declaration);

    ReDeclarationException(const ReDeclarationException &reference);

    ~ReDeclarationException() override;
};


#endif //APE_LANG_COMPILER_REDECLARATIONEXCEPTION_H
