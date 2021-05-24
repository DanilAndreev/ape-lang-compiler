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

#ifndef APE_LANG_COMPILER_COMPILER_H
#define APE_LANG_COMPILER_COMPILER_H


#include "../Tokenizer/Nodes/Node.h"
#include <vector>
#include <sstream>

class Compiler {
public:
    enum COMMANDS {
        FETCH,
        STORE,
        PUSH,
        POP,
        ADD,
        SUBTRACT,
        MULTIPLY,
        DIVIDE,
        POWER,
        LT,
        LTE,
        AND,
        OR,
        EQ,
        NEQ,
        JZ,
        JNZ,
        JMP,
        HALT,
        PRINT,
        SCAN,
        CONVERT,
    };
protected:
    std::vector<std::string> program;
    unsigned long long address;
public:
    explicit Compiler();

    template <class T>
    void generate(T command);

    std::vector<std::string> compile(std::shared_ptr<Node> tree);
protected:
    void compile_tree(std::shared_ptr<Node> tree);
};


#endif //APE_LANG_COMPILER_COMPILER_H
