#include <iostream>
#include <fstream>
#include "Lexer/Lexer.h"
#include "Tokenizer/Node.h"
#include "Tokenizer/Tokenizer.h"
#include "exceptions/ApeCompilerException.h"

using namespace std;

int main(int _argc, char *_argv[]) {
    ifstream *fin = new ifstream("test.txt", std::ios::binary);

    if (!fin->is_open()) {
        cerr << "Failed to open file";
        return 1;
    }

    Lexer *lexer = new Lexer(fin);

//    shared_ptr<Token> token = nullptr;
//    do {
//        token = lexer->nextToken();
//        string payload = token->getPayload() == "\n" ? "\\n" : token->getPayload();
//        cout << token->getType() << " \"" << payload << "\"" << endl;
//    } while (token->getType() != Token::TYPE::EOFILE);

    Tokenizer *tokenizer = new Tokenizer(lexer);
    cout << "parsing" << endl;
    shared_ptr<Node> tree = tokenizer->parse();
    cout << "parsed" << endl;
    tree->print(cout, 0, "root");

    shared_ptr<vector<ApeCompilerException>> errors;
    try {
        errors = Tokenizer::validateTree(tree).second;
        for (const auto& error: *errors) {
            cerr << "Compilation error: " << error.getMessage() << endl;
        }
    } catch (ApeCompilerException &e) {
        cerr << "Compilation error: " << e.getMessage() << endl;
    } catch (...) {
        cerr << "Unrecognized error" << endl;
    }

    tree->destructTree();
    tree = nullptr;


    fin->close();
    delete lexer;
    delete fin;

    return 0;
}
