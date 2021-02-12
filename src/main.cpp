#include <iostream>
#include <fstream>
#include "Lexer/Lexer.h"

using namespace std;

int main(int _argc, char *_argv[]) {
    ifstream *fin = new ifstream("test.txt", std::ios::binary);

    if (!fin->is_open()) {
        cerr << "Failed to open file";
        return 1;
    }

    Lexer *lexer = new Lexer(fin);

    Token *token = nullptr;
    do {
        Token result = lexer->nextToken();
        token = &result;
        string payload = token->getPayload() == "\n" ? "\\n" : token->getPayload();
        cout << token->getType() << " \"" << payload << "\"" << endl;
    } while (token->getType() != Token::TYPE::EOFILE);


    fin->close();
    delete lexer;
    delete fin;

    return 0;
}
