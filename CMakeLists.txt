cmake_minimum_required(VERSION 3.17)
project(ape_lang_compiler)

set(CMAKE_CXX_STANDARD 17)

add_executable(ape_lang_compiler src/main.cpp src/Lexer/Lexer.cpp src/Lexer/Lexer.h src/Lexer/Token.cpp src/Lexer/Token.h src/Lexer/NumberToken.cpp src/Lexer/NumberToken.h src/Tokenizer/Tokenizer.cpp src/Tokenizer/Tokenizer.h src/Tokenizer/Node.cpp src/Tokenizer/Node.h src/Lexer/OperatorToken.cpp src/Lexer/OperatorToken.h src/Lexer/KeywordToken.cpp src/Lexer/KeywordToken.h src/exceptions/ApeCompilerException.cpp src/exceptions/ApeCompilerException.h src/exceptions/SyntaxException.cpp src/exceptions/SyntaxException.h src/Tokenizer/LiteralNode.cpp src/Tokenizer/LiteralNode.h src/Tokenizer/StringNode.cpp src/Tokenizer/StringNode.h src/Tokenizer/IntegerNode.cpp src/Tokenizer/IntegerNode.h src/Tokenizer/FloatNode.cpp src/Tokenizer/FloatNode.h src/Tokenizer/VariableNode.cpp src/Tokenizer/VariableNode.h)