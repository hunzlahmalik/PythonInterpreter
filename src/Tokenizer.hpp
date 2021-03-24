//
// Created by Ali A. Kooshesh on 1/30/19.
//

#ifndef EXPRINTER_TOKENIZER_HPP
#define EXPRINTER_TOKENIZER_HPP

#include <fstream>
#include <vector>
#include "Token.hpp"

class Tokenizer
{
public:
    Tokenizer(std::ifstream &inStream);
    Token getToken();
    void ungetToken();
    void printProcessedTokens();
    Token createDedent();

    std::vector<Token> _tokens;

private:
    Token lastToken;
    bool ungottenToken;
    std::ifstream &inStream;
    bool parsingANewLine;
    int indent;
    int indentLevel;
    std::vector<int> dedentTracker;

private:
    std::string readName();
    int readInteger();
};

#endif //EXPRINTER_TOKENIZER_HPP
