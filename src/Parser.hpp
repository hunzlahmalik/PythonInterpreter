/**
 * @file Parser.hpp
 * @brief Major file for parsing the token from lines
 * @version 0.1
 */
#ifndef __PARSER_HPP
#define __PARSER_HPP

#include "Token.hpp"
#include "Tokenizer.hpp"
#include "SymTab.hpp"
#include "Expr.hpp"
#include "Statements.hpp"

#include <vector>
#include <iostream>
#include <map>

class Parser
{
public:
    Parser(Tokenizer &tokenizer) : tokenizer{tokenizer} {}

    /**
     * @brief we call this function when we know there is something \
     * to parse
     * 
     * @return Statements* 
     */
    Statements *statements();

    // function to use when there is the assignment statement with '='
    Statement *assignStatement();

    // function to parse everythin in the indention. Used when there is def/ indentation series
    Statements *suite(bool lambda = false);
    //Statements *func_suite();

    //same as the suite()
    Statements *func_body();

    /**
     * @brief creates a token that represents a true value and checks if the next token is a colon
     * 
     * @param ternary | used for the ternary operator
     * @return ExprNode* | expression node we get
     */
    ExprNode *returnTrue(bool ternary = false);

    std::vector<ExprNode *> testList();

    //used to get the parameters of lambda and def fucntion
    std::vector<std::string> parameter_list();

    //test of expression
    ExprNode *test();
    ExprNode *or_test();
    ExprNode *and_test();
    ExprNode *not_test();

    // to get array length
    ExprNode *array_len();
    Statement *array_op(Token varName);

    ExprNode *relTerm();

    ExprNode *expr();
    ExprNode *term();
    ExprNode *primary();

    std::string id();

private:
    Tokenizer &tokenizer;                                           //contains tokens
    std::vector<std::string> functions;                             //contains functions
    void die(std::string where, std::string message, Token &token); //called when there is error
};

#endif
