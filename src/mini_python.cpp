/**
 * @file main.cpp
 * @author Victor
 * @brief main file for mini python interpreter
 * @version 0.1
 * 
 */

#include <iostream>
#include <vector>

#include "Token.hpp"
#include "Tokenizer.hpp"
#include "Parser.hpp"

extern bool debug(false);

/**
 * @brief main function for mini_python interpreter
 * 
 * @param argc argument count
 * @param argv arguments
 * @return int 
 */
int main(int argc, char *argv[])
{

    /*if( argc != 2) {
        std::cout << "usage: " << argv[0] << " input file name\n";
        exit(1);
    }*/

    std::ifstream inputStream;
    char file[] = "../tests/tc4.py";

    inputStream.open(file, std::ios::in);
    if (!inputStream.is_open())
    {
        std::cout << "Unable top open " << argv[1] << ". Terminating...";
        perror("Error when attempting to open the input file.");
        exit(2);
    }

    Tokenizer tokenizer(inputStream);
    Parser parser(tokenizer);
    Statements *statements = parser.statements();
    SymTab symTab;

    //statements->print();
    statements->evaluate(symTab);
    delete statements;
    return 0;
}
