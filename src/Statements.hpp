#ifndef EXPRINTER_STATEMENTS_HPP
#define EXPRINTER_STATEMENTS_HPP

#include <iostream>
#include <vector>

#include "Expr.hpp"
#include "SymTab.hpp"

// pure-virutal class
class Statement
{
public:
    Statement();
    ~Statement();

    virtual void print() = 0;
    virtual void evaluate(SymTab &symTab) = 0;
    void setIndentLevel(int indent) { indentLevel = indent; }
    int getIndentLevel() { return indentLevel; }

private:
    int indentLevel;
};

//list of statements and a statement can be represented as on of python terms
class Statements
{
public:
    Statements();
    ~Statements();

    //addStatement to the list of Statements
    void addStatement(Statement *statement);

    //runs all statements
    void evaluate(SymTab &symTab);

    //print function for all the sattemnts
    void print();

private:
    std::vector<Statement *> _statements;
};

// AssignmentStatement represents the notion of an lValue having been assigned an rValue.
// The rValue is an expression.
class AssignmentStatement : public Statement
{
public:
    AssignmentStatement();
    ~AssignmentStatement();

    //makes an assignment statement
    AssignmentStatement(std::string lhsVar, ExprNode *rhsExpr);

    //return the left variable name
    std::string &lhsVariable();

    //returns the right side expression
    ExprNode *&rhsExpression();

    virtual void evaluate(SymTab &symTab);
    virtual void print();

private:
    std::string _lhsVariable;
    ExprNode *_rhsExpression;
};

//array assignment operator
class ArrAssignmentStatement : public Statement
{
public:
    ArrAssignmentStatement();
    ~ArrAssignmentStatement();

    //makes new array assignment
    ArrAssignmentStatement(std::string lhsVar, std::vector<ExprNode *> rhsArray);

    std::string &lhsVariable();

    //returnt the right side list of expressions
    std::vector<ExprNode *> &rhsArray();

    virtual void evaluate(SymTab &symTab);
    virtual void print();

private:
    std::string _lhsVariable;
    std::vector<ExprNode *> _rhsArray;
};

class ArrAccessAssign : public Statement
{
public:
    ArrAccessAssign();
    ~ArrAccessAssign();
    ArrAccessAssign(std::string lhs, int i, ExprNode *rhs);

    virtual void evaluate(SymTab &symTab);
    virtual void print();

private:
    std::string _lhsVariable;
    int index;
    ExprNode *_rhsExpression;
};

class ArrOpPop : public Statement
{
public:
    ArrOpPop();
    ~ArrOpPop();
    ArrOpPop(std::string arrName);

    virtual void evaluate(SymTab &symTab);
    virtual void print();

private:
    std::string _arrName;
};

//append funciton
class ArrOpAppend : public Statement
{
public:
    ArrOpAppend();
    ~ArrOpAppend();
    ArrOpAppend(std::string arrName, ExprNode *test);

    virtual void evaluate(SymTab &symTab);
    virtual void print();

private:
    ExprNode *_test;
    std::string _arrName;
};

// one of main statements
// print statement is for about every varaible and functions
class PrintStatement : public Statement
{
public:
    PrintStatement();
    ~PrintStatement();
    PrintStatement(std::vector<ExprNode *> mess);

    virtual void evaluate(SymTab &symTab);
    virtual void print();

private:
    std::vector<ExprNode *> message;
};

class ForStatement : public Statement
{
public:
    ForStatement();
    ~ForStatement();
    ForStatement(Token tok, std::vector<ExprNode *> range, Statements *b);

    virtual void evaluate(SymTab &symTab);
    virtual void print();
    bool range();

private:
    int lower, upper, increment;
    bool initLow, initUpper, initIncrement;
    std::vector<ExprNode *> _range;
    Token name;
    Statements *body;
};

//node for the if statements
class IfNode : public Statement
{
public:
    IfNode();
    ~IfNode();
    IfNode(ExprNode *rel, Statements *bod);

    virtual void evaluate(SymTab &symTab);
    virtual void print();
    bool evaluated();

private:
    ExprNode *relNode;
    Statements *body;
    bool evaluatedTrue;
};

//collective list of IFNodes
class IfStatement : public Statement
{
public:
    IfStatement();
    ~IfStatement();
    IfStatement(std::vector<IfNode *> ifs);

    virtual void evaluate(SymTab &symTab);
    virtual void print();

private:
    std::vector<IfNode *> body;
};

//is stored inside of the functions class
class FunctionStatement : public Statement
{
public:
    FunctionStatement();
    ~FunctionStatement();
    FunctionStatement(std::vector<std::string> param, Statements *s, std::string _name);

    virtual void evaluate(SymTab &symTab);
    virtual void print();
    std::string getParameterName(int index);

private:
    Statements *suite;
    std::vector<std::string> parameters;
    std::string _name;
};

//Defines a new function
class FunctionDef : public Statement
{
public:
    FunctionDef();
    FunctionDef(std::vector<std::string> param, Statements *s, std::string _name);
    ~FunctionDef();
    virtual void evaluate(SymTab &symTab);
    virtual void print();

private:
    Statements *suite;
    std::vector<std::string> parameters;
    std::string _name;
};

class FunctionCall : public Statement
{
public:
    FunctionCall(ExprNode *call);
    ~FunctionCall();

    virtual void print();
    virtual void evaluate(SymTab &symTab);

private:
    ExprNode *_call;
};

//'return' statement
class ReturnStatement : public Statement
{
public:
    ReturnStatement(ExprNode *ret);
    ~ReturnStatement();

    virtual void print();
    virtual void evaluate(SymTab &symTab);

private:
    ExprNode *_return;
};

//Stores all of our functions
class Functions
{
public:
    Functions();
    ~Functions();
    void addFunction(std::string name, FunctionStatement *func);
    bool isDefined(std::string name);
    FunctionStatement *findFunction(std::string funcName);

private:
    std::map<std::string, FunctionStatement *> functions;
};

#endif //EXPRINTER_STATEMENTS_HPP
