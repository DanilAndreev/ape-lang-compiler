#include <iostream>
#include <fstream>
#include "Lexer/Lexer.h"
#include "Tokenizer/Node.h"
#include "Tokenizer/Tokenizer.h"
#include "exceptions/ApeCompilerException.h"
#include "Compiler/Compiler.h"


using namespace std;

vector<string> compile(ifstream& fin) {
    Lexer *lexer = new Lexer(&fin);
    Tokenizer *tokenizer = new Tokenizer(lexer);
    shared_ptr<Node> tree = tokenizer->parse();

#ifndef NDEBUG
    tree->print(cout, 0, "root");
#endif

    shared_ptr<vector<ApeCompilerException>> errors;
    try {
        errors = get<1>(Tokenizer::validateTree(tree));
        for (const auto& error: *errors) {
            cerr << "Compilation error: " << error.getMessage() << endl;
        }
        if (errors->size())
            exit(-1);
    } catch (ApeCompilerException &e) {
        cerr << "Compilation error: " << e.getMessage() << endl;
    } catch (...) {
        cerr << "Unrecognized error" << endl;
    }

#ifndef NDEBUG
    cout << "--- AFTER VALIDATION ---" << endl;
    tree->print(cout, 0, "root");
#endif

    Compiler* compiler = new Compiler();
    vector<string> program = compiler->compile(tree);


    tree->destructTree();
    tree = nullptr;
    delete tokenizer;
    delete lexer;
    delete compiler;

    return program;
}

int main(int _argc, char *_argv[]) {
    vector<string> args(_argv + 1, _argv + _argc);

    string inputFile;
    string resultFile = "a.ape";

    if (args.empty()) {
        cerr <<"Not enough arguments, expected: <file> [result] " << endl;
        return -1;
    }
    inputFile = args[0];

    if (args.size() >= 2)
        resultFile = args[1];

    if (args.size() > 2) {
        cerr <<"Too many arguments, expected: <file> [result] " << endl;
        return -1;
    }

    ifstream fin(inputFile, std::ios::binary);

    if (!fin.is_open()) {
        cerr << "Failed to open file: \"" << inputFile << "\"" << endl;
        return -2;
    }


    vector<string> program = compile(fin);
    fin.close();

    ofstream fout(resultFile);
    if (!fout.is_open()) {
        cerr << "Failed to open or create file: \"" << resultFile << "\"" << endl;
        return -2;
    }


    fout << "[";
    for (string& item : program) {
        fout << item << ",";
    }
    fout << "]" << endl;

    return 0;
}
