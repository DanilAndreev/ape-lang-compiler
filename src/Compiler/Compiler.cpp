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

#include "Compiler.h"
#include "../Tokenizer/VariableNode.h"
#include "../Tokenizer/IntegerNode.h"
#include "../Tokenizer/StringNode.h"
#include "../Tokenizer/FloatNode.h"
#include "../Tokenizer/DeclarationNode.h"

Compiler::Compiler() {
    this->address = 0;
}

void Compiler::compile_tree(shared_ptr<Node> tree) {
    switch (tree->getType()) {
        case Node::VAR: {
            shared_ptr<DeclarationNode> declaration = dynamic_pointer_cast<DeclarationNode>(tree);
            if (declaration != nullptr) {
                this->compile_tree(tree->getOperand1());
                this->generate(COMMANDS::STORE);
                string identifier = "\"";
                identifier += declaration->getIdentifier();
                identifier += "\"";
                this->generate(identifier);
            } else {
                this->generate(COMMANDS::FETCH);
                shared_ptr<VariableNode> node = dynamic_pointer_cast<VariableNode>(tree);
                string identifier = "\"";
                identifier += node->getIdentifier();
                identifier += "\"";
                this->generate(identifier);
            }
        }
            break;
        case Node::CONST: {
            this->generate(COMMANDS::PUSH);
            if (shared_ptr<IntegerNode> node = dynamic_pointer_cast<IntegerNode>(tree)) {
                this->generate(node->getPayload());
            } else if (shared_ptr<StringNode> node = dynamic_pointer_cast<StringNode>(tree)) {
                this->generate(node->getPayload());
            } else if (shared_ptr<FloatNode> node = dynamic_pointer_cast<FloatNode>(tree)) {
                this->generate(node->getPayload());
            }
        }
            break;
        case Node::ADD:
            this->compile_tree(tree->getOperand1());
            this->compile_tree(tree->getOperand2());
            this->generate(COMMANDS::ADD);
            break;
        case Node::SUBTRACT:
            this->compile_tree(tree->getOperand1());
            this->compile_tree(tree->getOperand2());
            this->generate(COMMANDS::SUBTRACT);
            break;
        case Node::MULTIPLY:
            this->compile_tree(tree->getOperand1());
            this->compile_tree(tree->getOperand2());
            this->generate(COMMANDS::MULTIPLY);
            break;
        case Node::DIVIDE:
            this->compile_tree(tree->getOperand1());
            this->compile_tree(tree->getOperand2());
            this->generate(COMMANDS::DIVIDE);
            break;
        case Node::LESS: {
            this->compile_tree(tree->getOperand1());
            this->compile_tree(tree->getOperand2());
            this->generate(COMMANDS::LT);
        }
            break;
        case Node::LESS_EQUAL:
            this->compile_tree(tree->getOperand1());
            this->compile_tree(tree->getOperand2());
            this->generate(COMMANDS::LTE);
            break;
        case Node::GREATER:
            this->compile_tree(tree->getOperand2());
            this->compile_tree(tree->getOperand1());
            this->generate(COMMANDS::LT);
            break;
        case Node::GREATER_EQUAL:
            this->compile_tree(tree->getOperand2());
            this->compile_tree(tree->getOperand1());
            this->generate(COMMANDS::LTE);
            break;
        case Node::EQUAL:
            this->compile_tree(tree->getOperand1());
            this->compile_tree(tree->getOperand2());
            this->generate(COMMANDS::EQ);
            break;
        case Node::NOT_EQUAL:
            this->compile_tree(tree->getOperand1());
            this->compile_tree(tree->getOperand2());
            this->generate(COMMANDS::NEQ);
            break;
        case Node::SET: {
            this->compile_tree(tree->getOperand2());
            this->generate(COMMANDS::STORE);
            shared_ptr<VariableNode> node = dynamic_pointer_cast<VariableNode>(tree->getOperand1());
            string identifier = "\"";
            identifier += node->getIdentifier();
            identifier += "\"";
            this->generate(identifier);
        }
            break;
        case Node::IF: {
            this->compile_tree(tree->getOperand1());
            this->generate(COMMANDS::JZ);
            unsigned long long jump_address = this->address;
            this->generate(0);
            this->compile_tree(tree->getOperand2());
            this->program[jump_address] = to_string(this->address);
        }
            break;
        case Node::IFELSE: {
            this->compile_tree(tree->getOperand1());
            this->generate(COMMANDS::JZ);
            unsigned long long jump_address1 = this->address;
            this->generate(0);
            this->compile_tree(tree->getOperand2());
            this->generate(COMMANDS::JMP);
            unsigned long long jump_address2 = this->address;
            this->generate(0);
            this->program[jump_address1] = to_string(this->address);
            this->compile_tree(tree->getOperand3());
            this->program[jump_address2] = to_string(this->address);
        }
            break;
        case Node::WHILE: {
            unsigned long long jump_address1 = this->address;
            this->compile_tree(tree->getOperand1());
            this->generate(COMMANDS::JZ);
            unsigned long long jump_address2 = this->address;
            this->generate(0);
            this->compile_tree(tree->getOperand2());
            this->generate(COMMANDS::JMP);
            this->generate(jump_address1);
            this->program[jump_address2] = to_string(this->address);
        }
            break;
        case Node::DO: {
            unsigned long long jump_address = this->address;
            this->compile_tree(tree->getOperand1());
            this->compile_tree(tree->getOperand2());
            this->generate(COMMANDS::JNZ);
            this->generate(jump_address);
        }
            break;
        case Node::SEQUENCE:
            this->compile_tree(tree->getOperand1());
            this->compile_tree(tree->getOperand2());
            break;
        case Node::EXPRESSION:
            this->compile_tree(tree->getOperand1());
            this->generate(COMMANDS::POP);
            break;
        case Node::PROGRAM:
            this->compile_tree(tree->getOperand1());
            this->generate(COMMANDS::HALT);
            break;
        case Node::EMPTY:
            break;
        case Node::SCOPE:
            this->compile_tree(tree->getOperand1());
            break;
        case Node::FOR:
            break;
    }
}

template<class T>
void Compiler::generate(T command) {
    ostringstream str;
    str << command;
    this->program.push_back(str.str());
    this->address++;
}

vector<string> Compiler::compile(shared_ptr<Node> tree) {
    this->compile_tree(tree);
    this->generate(COMMANDS::HALT);
    return this->program;
}