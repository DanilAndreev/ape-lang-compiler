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

#include <cstring>
#include "ApeCompilerException.h"

using namespace std;

std::string ApeCompilerException::getMessage() const noexcept {
    return this->message;
}

ApeCompilerException::ApeCompilerException(const ApeCompilerException &reference): std::exception(reference) {
    this->fatal = reference.fatal;
    this->message = reference.message;
}

ApeCompilerException::ApeCompilerException(std::string message, bool fatal): std::exception() {
    this->message = message;
    this->fatal = fatal;
}

const char *ApeCompilerException::what() const noexcept {
    return "ApeCompilerException, use getMessage() to get message.";
}

ApeCompilerException::~ApeCompilerException() {}

std::ostream &operator<<(std::ostream &stream, ApeCompilerException &exception) {
    stream << exception.getMessage();
    return stream;
}
