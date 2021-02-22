#include <iostream>
#include <fstream>
#include "Lexer/Lexer.h"
#include "Tokenizer/Node.h"
#include "Tokenizer/Tokenizer.h"

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
    Node *tree = tokenizer->parse();
    tree->print(cout, 0, "root");
    tree->destructTree();
    delete tree;


    fin->close();
    delete lexer;
    delete fin;





//    Node* root = new Node(Node::TYPE::FOR);
//    Node* op1 = new Node(Node::TYPE::IF);
//    op1->setOperand1(new Node(Node::TYPE::FOR));
//
//    root->setOperand1(op1);
//    root->setOperand2(new Node(Node::TYPE::SUB));
//    root->setOperand3(new Node(Node::TYPE::ADD));
//
//    root->print(cout, 0, "root");
//
//    root->destructTree();
//    delete root;

    return 0;
}
