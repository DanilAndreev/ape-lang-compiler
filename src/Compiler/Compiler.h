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

/**
 * Compiler - class for compiling AST to ApeRPN.
 * @class
 * @author Danil Andreev
 */
class Compiler {
public:
    /**
     * COMMANDS - ApeVM commands.
     * @enum
     * @author Danil Andreev
     */
    enum COMMANDS {
        /// FETCH A | Put value of variable A to the top of the stack.
        FETCH,
        /// STORE A | Put value to variable A from the top of the stack.
        STORE,
        /// PUSH N | Put value N to the top of the stack.
        PUSH,
        /// POP | Remove value from the top of the stack.
        POP,
        /// ADD | Add two variables from the top of the stack.
        ADD,
        /// SUBTRACT | Subtract value before the top of the stack from value on the top of the stack.
        SUBTRACT,
        /// MULTIPLY | Multiply two variables from the top of the stack.
        MULTIPLY,
        /// DIVIDE | Divide value before the top of the stack on value on the top of the stack.
        DIVIDE,
        /// POWER | Calculate value from the top of the stack to power value before the top of the stack.
        POWER,
        /// LT | Compare two values on the top of the stack a < b.
        LT,
        /// LTE | Compare two values on the top of the stack a <= b.
        LTE,
        /// AND | Logical operation AND with two values on the top of the stack.
        AND,
        /// OR | Logical operation OR with two values on the top of the stack.
        OR,
        /// EQ | Compare two values on the top of the stack a = b.
        EQ,
        /// NEQ | Compare two values on the top of the stack a != b.
        NEQ,
        /// JZ A | Jump to address A if there is 0 on the top of the stack.
        JZ,
        /// JNZ A | Jump to address A if there is not 0 on the top of the stack.
        JNZ,
        /// JMP A | Jump to address A.
        JMP,
        /// HALT | Finish work.
        HALT,
        /// PRINT | Print value from the top of the stack to stdout.
        PRINT,
        /// SCAN A | Scan value of type A from stdin and put result to the top of the stack.
        SCAN,
        /// CONVERT A | Convert value on the top of the stack to type A.
        CONVERT,
    };
protected:
    /// program - Compiled program. Tokens will be appended to the end of vector.
    std::vector<std::string> program;
    /// address - current address in the program.
    unsigned long long address;
public:
    explicit Compiler();

    /**
     * generate - generates next command to the program.
     * @param command - target command or value.
     * @author Danil Andreev
     */
    template<class T>
    void generate(T command);

    /**
     * compile - compiles AST to ApeRPN.
     * @param tree - root node of AST.
     * @author Danil Anreev
     */
    std::vector<std::string> compile(std::shared_ptr<Node> tree);

protected:
    /**
     * compile_tree - recursively goes over AST and compiles it.
     * @note Uses recursion.
     * @param tree - root node of AST.
     * @author Danil Anreev
     */
    void compile_tree(std::shared_ptr<Node> tree);
};


#endif //APE_LANG_COMPILER_COMPILER_H
