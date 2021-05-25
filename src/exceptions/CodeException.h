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

#ifndef APE_LANG_COMPILER_CODEEXCEPTION_H
#define APE_LANG_COMPILER_CODEEXCEPTION_H

#include <memory>
#include "ApeCompilerException.h"
#include "../interfaces/Positionable.h"

/**
 * CodeException - exception class for errors in code. Contains error localization.
 * @class
 * @author Danil Andreev
 */
class CodeException : public ApeCompilerException, public Positionable {
public:
    CodeException(int line, int column, std::string message);

    CodeException(std::shared_ptr<Positionable> target, std::string message);

    CodeException(const CodeException &reference);

    ~CodeException() override;

public:
    std::string getMessage() const noexcept override;
};


#endif //APE_LANG_COMPILER_CODEEXCEPTION_H
