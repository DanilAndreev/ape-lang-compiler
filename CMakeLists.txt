cmake_minimum_required(VERSION 3.17)
project(ape_lang_compiler)

set(CMAKE_CXX_STANDARD 17)

add_executable(ape_lang_compiler src/main.cpp src/Lexer.cpp src/Lexer.h src/Token.cpp src/Token.h src/NumberToken.cpp src/NumberToken.h)