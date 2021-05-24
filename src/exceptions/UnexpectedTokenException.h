//
// Created by danil on 24.05.21.
//

#ifndef APE_LANG_COMPILER_UNEXPECTEDTOKENEXCEPTION_H
#define APE_LANG_COMPILER_UNEXPECTEDTOKENEXCEPTION_H


#include "CodeException.h"
#include "../Lexer/Tokens/Token.h"

class UnexpectedTokenException : public CodeException {
public:
    UnexpectedTokenException(std::shared_ptr<Token> got, std::string expected);
    UnexpectedTokenException(const UnexpectedTokenException& reference);
    ~UnexpectedTokenException() override;
};


#endif //APE_LANG_COMPILER_UNEXPECTEDTOKENEXCEPTION_H
