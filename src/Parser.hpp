
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
    Parser(Tokenizer &tokenizer) : tokenizer{tokenizer}
    {
    }

    Statements *statements();

    Statement *assignStatement();

    Statements *suite(bool lambda =false);
    //Statements *func_suite();
    Statements *func_body();
    ExprNode *returnTrue(bool ternary = false);

    std::vector<ExprNode *> testList();
    std::vector<std::string> parameter_list();
    ExprNode *test();
    ExprNode *or_test();
    ExprNode *and_test();
    ExprNode *not_test();
    ExprNode *array_len();
    Statement *array_op(Token varName);

    ExprNode *relTerm();

    ExprNode *expr();
    ExprNode *term();
    ExprNode *primary();

    std::string id();

private:
    Tokenizer &tokenizer;
    std::vector<std::string> functions;
    void die(std::string where, std::string message, Token &token);
};

#endif
