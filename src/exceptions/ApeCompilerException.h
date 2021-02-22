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

#ifndef APE_LANG_COMPILER_APECOMPILEREXCEPTION_H
#define APE_LANG_COMPILER_APECOMPILEREXCEPTION_H

#include <exception>
#include <string>
#include <iostream>

/**
 * ApeCompilerException - base class for ApeLang exceptions.
 * @author Danil Andreev
 */
class ApeCompilerException : public std::exception {
    /// fatal - if true error will be interpreted as fatal.
    bool fatal;
    /// message - error message.
    std::string message;
public:
    explicit ApeCompilerException(std::string message = "", bool fatal = false);
    ApeCompilerException(const ApeCompilerException& reference);
    ~ApeCompilerException() override;
public:
    /**
     * getMessage - getter for exception message.
     * @author Danil Andreev
     */
    std::string getMessage() const noexcept;
    const char* what() const noexcept override;
public:
    friend std::ostream& operator<< (std::ostream& stream, ApeCompilerException& exception);
};


#endif //APE_LANG_COMPILER_APECOMPILEREXCEPTION_H
