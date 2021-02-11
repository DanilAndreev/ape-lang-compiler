cmake_minimum_required(VERSION 3.17)
project(ape_lang_compiler)

set(CMAKE_CXX_STANDARD 17)

add_executable(ape_lang_compiler src/Lexer/main.cpp src/Lexer/Lexer.cpp src/Lexer/Lexer.h src/Lexer/Token.cpp src/Lexer/Token.h src/Lexer/NumberToken.cpp src/Lexer/NumberToken.h)