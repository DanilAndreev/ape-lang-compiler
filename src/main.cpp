#include <iostream>
#include <fstream>
#include "Lexer.h"

using namespace std;

int main(int _argc, char* _argv[]) {
    ifstream* fin = new ifstream("test.txt");

    if (!fin->is_open()) {
        cerr << "Failed to open file";
        return 1;
    }

    Lexer* lexer = new Lexer(fin);
    wcout << lexer->nextToken().getPayload();

    cout << "Hello, World!" << endl;
    return 0;
}
