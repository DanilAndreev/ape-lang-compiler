#include <iostream>
#include <fstream>
#include "Lexer/Lexer.h"
#include "Tokenizer/Node.h"
#include "Tokenizer/Tokenizer.h"
#include "exceptions/ApeCompilerException.h"
#include "Compiler/Compiler.h"


using namespace std;

vector<string> compile(ifstream& fin) {

}

int main(int _argc, char *_argv[]) {
    vector<string> args(_argv + 1, _argv + _argc);




    ifstream *fin = new ifstream("test.txt", std::ios::binary);

    if (!fin->is_open()) {
        cerr << "Failed to open file";
        return 1;
    }

    Lexer *lexer = new Lexer(fin);
    Tokenizer *tokenizer = new Tokenizer(lexer);
    cout << "parsing" << endl;
    shared_ptr<Node> tree = tokenizer->parse();
    cout << "parsed" << endl;
    #ifndef NDEBUG
    tree->print(cout, 0, "root");
    #endif

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


    Compiler* compiler = new Compiler();
    vector<string> program = compiler->compile(tree);

    cout << "[";
    for (string& item : program) {
        cout << item << ",";
    }
    cout << "]" << endl;

    tree->destructTree();
    tree = nullptr;

    fin->close();
    delete compiler;
    delete lexer;
    delete fin;

    return 0;
}
