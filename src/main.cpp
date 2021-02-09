#include <iostream>
#include <fstream>
#include "Lexer.h"

#include <regex>


using namespace std;

int main(int _argc, char *_argv[]) {
//    wregex rx(L"\\+\\-");
//    wchar_t ch = L's';
//    cout << regex_search(&ch, rx) << endl;
//
//    system("pause");


    wifstream *fin = new wifstream("test.txt");

    if (!fin->is_open()) {
        cerr << "Failed to open file";
        return 1;
    }

    Lexer *lexer = new Lexer(fin);

    Token *token = nullptr;
    do {
        Token result = lexer->nextToken();
        token = &result;
        wstring payload = token->getPayload() == L"\n" ? L"\\n" : token->getPayload();
        wcout << token->getType() << " \"" << payload << "\"" << endl;
    } while (token->getType() != Token::TYPE::EOFILE);


    fin->close();
    delete lexer;
    delete fin;

    return 0;
}
