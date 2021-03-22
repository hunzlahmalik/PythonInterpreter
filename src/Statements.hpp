//
// Created by Ali A. Kooshesh on 2/5/19.
//

#ifndef EXPRINTER_STATEMENTS_HPP
#define EXPRINTER_STATEMENTS_HPP

#include <iostream>
#include <vector>

#include "Expr.hpp"
#include "SymTab.hpp"

// The Statement (abstract) class serves as a super class for all statements that
// are defined in the language. Ultimately, statements have to be evaluated.
// Therefore, this class defines evaluate, a pure-virtual function to make
// sure that all subclasses of Statement provide an implementation for this
// function.

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

// Statements is a collection of Statement. For example, all statements in a function
// can be represented by an instance of Statements.

class Statements
{
public:
    Statements();
    ~Statements();

    void addStatement(Statement *statement);
    void evaluate(SymTab &symTab);

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
    AssignmentStatement(std::string lhsVar, ExprNode *rhsExpr);

    std::string &lhsVariable();
    ExprNode *&rhsExpression();

    virtual void evaluate(SymTab &symTab);
    virtual void print();

private:
    std::string _lhsVariable;
    ExprNode *_rhsExpression;
};

class ArrAssignmentStatement : public Statement
{
public:
    ArrAssignmentStatement();
    ~ArrAssignmentStatement();
    ArrAssignmentStatement(std::string lhsVar, std::vector<ExprNode *> rhsArray);

    std::string &lhsVariable();
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
