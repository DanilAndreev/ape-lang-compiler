//
// Created by danil on 24.05.21.
//

#include <sstream>
#include "UnexpectedTokenException.h"

using namespace std;

UnexpectedTokenException::UnexpectedTokenException(std::shared_ptr<Token> got, std::string expected)
        : CodeException(got->getLine(), got->getColumn(), "") {
    ostringstream text;
    text << "Unexpected token \"" << got->getPayload() << "\", expected \"" << expected << "\".";
    this->message = text.str();
}

UnexpectedTokenException::UnexpectedTokenException(const UnexpectedTokenException &reference)
        : CodeException(reference) {
}

UnexpectedTokenException::~UnexpectedTokenException() {
}
