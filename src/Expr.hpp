//
// Created by Ali A. Kooshesh on 2/5/19.
//

#ifndef EXPRINTER_ARITHEXPR_HPP
#define EXPRINTER_ARITHEXPR_HPP

#include "Token.hpp"
#include "SymTab.hpp"

// Classes in this file define the internal representation of arithmetic expressions.

// An ExprNode serves as the base class (super class) for arithmetic expression.
// It forces the derived classes (subclasses) to implement two functions, print and
// evaluate.
class ExprNode
{
public:
    ExprNode(Token token);
    Token token();
    virtual void print() = 0;
    virtual TypeDescriptor *evaluate(SymTab &symTab) = 0;

private:
    Token _token;
};

// An InfixExprNode is useful to represent binary arithmetic operators.
class InfixExprNode : public ExprNode
{ // An expression tree node.

public:
    InfixExprNode(Token tk);
    ~InfixExprNode();
    ExprNode *&left();
    ExprNode *&right();
    virtual void print();
    virtual TypeDescriptor *evaluate(SymTab &symTab);

private:
    ExprNode *_left, *_right;
};

//Since InfixExprNode requires a left and right value, Not required its own class
class NotNode : public ExprNode
{
public:
    NotNode(Token tk); // init
    ~NotNode();        // destructor
    ExprNode *&rel();  // TODO ask Sean
    virtual void print();
    virtual TypeDescriptor *evaluate(SymTab &symTab);

private:
    ExprNode *_rel;
};

// WholeNumber is a leaf-node in an expression tree. It corresponds to
// a terminal in the production rules of the grammar that describes the
// syntax of arithmetic expressions.

class WholeNumber : public ExprNode
{
public:
    WholeNumber(Token token);
    virtual void print();
    virtual TypeDescriptor *evaluate(SymTab &symTab);
};

// Variable is a leaf-node in an expression tree. It corresponds to
// a terminal in the production rules of the grammar that describes the
// syntax of arithmetic expressions.

class Variable : public ExprNode
{
public:
    Variable(Token token);
    virtual void print();
    virtual TypeDescriptor *evaluate(SymTab &symTab);
};

class StringNode : public ExprNode
{
public:
    StringNode(Token token);
    virtual void print();
    virtual TypeDescriptor *evaluate(SymTab &symTab);
};

class ArrayNode : public ExprNode
{
public:
    ArrayNode(Token token, ExprNode *subscript, bool slice=false);
    ~ArrayNode();
    virtual void print();
    virtual TypeDescriptor *evaluate(SymTab &symTab);

private:
    ExprNode *_subscript;
    bool _slice;
};

class ArrayLen : public ExprNode
{
public:
    ArrayLen(Token token);
    virtual void print();
    virtual TypeDescriptor *evaluate(SymTab &symTab);
};

class CallNode : public ExprNode
{
public:
    CallNode(Token token, std::vector<ExprNode *> param);
    virtual void print();
    virtual TypeDescriptor *evaluate(SymTab &symTab);

private:
    std::vector<ExprNode *> parameters;
};

#endif //EXPRINTER_ARITHEXPR_HPP
