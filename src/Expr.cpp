//
// Created by Ali A. Kooshesh on 2/5/19.
//

#include <iostream>
#include "Expr.hpp"
#include "SymTab.hpp"
#include "Statements.hpp"

extern bool debug;

// ExprNode
ExprNode::ExprNode(Token token) : _token{token} {}

Token ExprNode::token() { return _token; }

// InfixExprNode functions
InfixExprNode::InfixExprNode(Token tk) : ExprNode{tk}, _left(nullptr), _right(nullptr) {}

InfixExprNode::~InfixExprNode()
{
    delete _left;
    delete _right;
}

ExprNode *&InfixExprNode::left() { return _left; }

ExprNode *&InfixExprNode::right() { return _right; }

TypeDescriptor *InfixExprNode::evaluate(SymTab &symTab)
{
    // Evaluates an infix expression using a post-order traversal of the expression tree.

    TypeDescriptor *lValue = left()->evaluate(symTab);
    TypeDescriptor *rValue = right()->evaluate(symTab);

    if (debug)
    {
        if (lValue->type() == TypeDescriptor::INTEGER)
            std::cout << "InfixExprNode::evaluate: " << dynamic_cast<NumberDescriptor *>(lValue)->value.intValue << " " << token().symbol() << " " << dynamic_cast<NumberDescriptor *>(rValue)->value.intValue << std::endl;
    }
    if (token().isAdditionOperator())
    {
        if (lValue->type() == TypeDescriptor::STRING)
        {
            StringDescriptor *result = new StringDescriptor(TypeDescriptor::STRING);
            result->value = dynamic_cast<StringDescriptor *>(lValue)->value + dynamic_cast<StringDescriptor *>(rValue)->value;
            return result;
        }
        else
        {
            NumberDescriptor *result = new NumberDescriptor(TypeDescriptor::INTEGER);
            result->value.intValue = dynamic_cast<NumberDescriptor *>(lValue)->value.intValue + dynamic_cast<NumberDescriptor *>(rValue)->value.intValue;
            return result;
        }
    }
    else if (token().isSubtractionOperator())
    {
        NumberDescriptor *result = new NumberDescriptor(TypeDescriptor::INTEGER);
        result->value.intValue = dynamic_cast<NumberDescriptor *>(lValue)->value.intValue - dynamic_cast<NumberDescriptor *>(rValue)->value.intValue;
        return result;
    }
    else if (token().isMultiplicationOperator())
    {
        NumberDescriptor *result = new NumberDescriptor(TypeDescriptor::INTEGER);
        result->value.intValue = dynamic_cast<NumberDescriptor *>(lValue)->value.intValue * dynamic_cast<NumberDescriptor *>(rValue)->value.intValue;
        return result;
    }
    else if (token().isDivisionOperator())
    {
        {
            NumberDescriptor *result = new NumberDescriptor(TypeDescriptor::INTEGER);
            result->value.intValue = dynamic_cast<NumberDescriptor *>(lValue)->value.intValue / dynamic_cast<NumberDescriptor *>(rValue)->value.intValue;
            return result;
        }
    }
    else if (token().isModuloOperator())
    {
        NumberDescriptor *result = new NumberDescriptor(TypeDescriptor::INTEGER);
        result->value.intValue = dynamic_cast<NumberDescriptor *>(lValue)->value.intValue % dynamic_cast<NumberDescriptor *>(rValue)->value.intValue;
        return result;
    }
    else if (token().isEqualSign())
    {
        NumberDescriptor *result = new NumberDescriptor(TypeDescriptor::BOOL);
        result->value.boolValue = dynamic_cast<NumberDescriptor *>(lValue)->value.intValue == dynamic_cast<NumberDescriptor *>(rValue)->value.intValue;
        return result;
    }
    else if (token().isNotEqualSign())
    {
        NumberDescriptor *result = new NumberDescriptor(TypeDescriptor::BOOL);
        result->value.boolValue = dynamic_cast<NumberDescriptor *>(lValue)->value.intValue != dynamic_cast<NumberDescriptor *>(rValue)->value.intValue;
        return result;
    }
    else if (token().isGreaterThanSign())
    {
        NumberDescriptor *result = new NumberDescriptor(TypeDescriptor::BOOL);
        result->value.boolValue = dynamic_cast<NumberDescriptor *>(lValue)->value.intValue > dynamic_cast<NumberDescriptor *>(rValue)->value.intValue;
        return result;
    }
    else if (token().isLessThanSign())
    {

        NumberDescriptor *result = new NumberDescriptor(TypeDescriptor::BOOL);
        result->value.boolValue = dynamic_cast<NumberDescriptor *>(lValue)->value.intValue < dynamic_cast<NumberDescriptor *>(rValue)->value.intValue;
        return result;
    }
    else if (token().isGreaterEqualThanSign())
    {
        NumberDescriptor *result = new NumberDescriptor(TypeDescriptor::BOOL);
        result->value.boolValue = dynamic_cast<NumberDescriptor *>(lValue)->value.intValue >= dynamic_cast<NumberDescriptor *>(rValue)->value.intValue;
        return result;
    }
    else if (token().isLessThanEqualSign())
    {
        NumberDescriptor *result = new NumberDescriptor(TypeDescriptor::BOOL);
        result->value.boolValue = dynamic_cast<NumberDescriptor *>(lValue)->value.intValue <= dynamic_cast<NumberDescriptor *>(rValue)->value.intValue;
        return result;
    }
    else if (token().isOr())
    {
        NumberDescriptor *result = new NumberDescriptor(TypeDescriptor::BOOL);
        result->value.boolValue = dynamic_cast<NumberDescriptor *>(lValue)->value.boolValue || dynamic_cast<NumberDescriptor *>(rValue)->value.boolValue;
        return result;
    }
    else if (token().isAnd())
    {
        NumberDescriptor *result = new NumberDescriptor(TypeDescriptor::BOOL);
        result->value.boolValue = dynamic_cast<NumberDescriptor *>(lValue)->value.boolValue && dynamic_cast<NumberDescriptor *>(rValue)->value.boolValue;
        return result;
    }
    else
    {
        if (debug)
            std::cout << "InfixExprNode::evaluate: don't know how to evaluate this operator\n";
        token().print();
        std::cout << std::endl;
        exit(2);
    }
}

void InfixExprNode::print()
{
    _left->print();
    token().print();
    _right->print();
}

//NotNode
//Since InfixExprNode requires a left and right value, Not required its own class
NotNode::NotNode(Token tk) : ExprNode{tk}, _rel{nullptr} {}

NotNode::~NotNode()
{
    delete _rel;
}

ExprNode *&NotNode::rel()
{
    return _rel;
}

void NotNode::print()
{
    std::cout << "not ";
    _rel->print();
}

TypeDescriptor *NotNode::evaluate(SymTab &symTab)
{
    NumberDescriptor *result = new NumberDescriptor(TypeDescriptor::BOOL);
    result->value.boolValue = !dynamic_cast<NumberDescriptor *>(_rel->evaluate(symTab))->value.boolValue;
    return result;
}

// WholeNumber
WholeNumber::WholeNumber(Token token) : ExprNode{token} {}

void WholeNumber::print()
{
    token().print();
}

TypeDescriptor *WholeNumber::evaluate(SymTab &symTab)
{
    if (debug)
        std::cout << "WholeNumber::evaluate: returning " << token().getWholeNumber() << std::endl;
    NumberDescriptor *temp = new NumberDescriptor(TypeDescriptor::INTEGER);
    temp->value.intValue = token().getWholeNumber();
    return temp;
}

// Variable

Variable::Variable(Token token) : ExprNode{token} {}

void Variable::print()
{
    token().print();
}

TypeDescriptor *Variable::evaluate(SymTab &symTab)
{
    if (!symTab.isDefined(token().getName()))
    {
        if (debug)
            std::cout << "Variable::evaluate: Use of undefined variable, " << token().getName() << std::endl;
        exit(1);
    }
    if (debug)
        std::cout << "Variable::evaluate: returning " << dynamic_cast<NumberDescriptor *>(symTab.getValueFor(token().getName()))->value.intValue << std::endl;
    return symTab.getValueFor(token().getName());
}

//String Node
StringNode::StringNode(Token token) : ExprNode{token} {}

void StringNode::print()
{
    std::cout << "\"";
    token().print();
    std::cout << "\"";
}

TypeDescriptor *StringNode::evaluate(SymTab &symTab)
{
    if (!symTab.isDefined(token().getName()))
    {
        if (debug)
            std::cout << "StringNode::evaluate: returning " << token().getString() << std::endl;
        StringDescriptor *temp = new StringDescriptor(TypeDescriptor::STRING);
        temp->value = token().getString();
        return temp;
    }
    if (debug)
        std::cout << "StringNode::evaluate: returning " << dynamic_cast<StringDescriptor *>(symTab.getValueFor(token().getName()))->value << std::endl;
    return dynamic_cast<StringDescriptor *>(symTab.getValueFor(token().getName()));
}

//Array Node
ArrayNode::ArrayNode(Token token, ExprNode *sub) : ExprNode{token}, _subscript{sub} {}

ArrayNode::~ArrayNode()
{
    delete _subscript;
}

void ArrayNode::print()
{
    token().print();
    std::cout << "[]" << std::endl;
}

TypeDescriptor *ArrayNode::evaluate(SymTab &symTab)
{
    if (symTab.isDefined(token().getName()))
    {
        if (debug)
            std::cout << "ArrayNode::evaluate: returning " << token().getString() << std::endl;
        ArrayDescriptor *temp = dynamic_cast<ArrayDescriptor *>(symTab.getValueFor(token().getName()));
        if (temp->type() == TypeDescriptor::INTEGER)
        {
            NumberDescriptor *num = dynamic_cast<NumberDescriptor *>(_subscript->evaluate(symTab));
            NumberDescriptor *returned = new NumberDescriptor(TypeDescriptor::INTEGER);
            returned->value.intValue = temp->valueInt[num->value.intValue];
            return returned;
        }

        NumberDescriptor *num = dynamic_cast<NumberDescriptor *>(_subscript->evaluate(symTab));
        StringDescriptor *returned = new StringDescriptor(TypeDescriptor::STRING);
        returned->value = temp->valueString[num->value.intValue];
        return returned;
    }
    if (debug)
        std::cout << "StringNode::evaluate: returning " << dynamic_cast<StringDescriptor *>(symTab.getValueFor(token().getName()))->value << std::endl;
    std::cout << " ArrayNode::evaluate, ERROR!!! Value does not exist" << std::endl;
    exit(1);
}

ArrayLen::ArrayLen(Token token) : ExprNode{token} {};

void ArrayLen::print()
{
    std::cout << "len(" << token().getName() << ")" << std::endl;
}

TypeDescriptor *ArrayLen::evaluate(SymTab &symTab)
{
    ArrayDescriptor *id;
    if (id = dynamic_cast<ArrayDescriptor *>(symTab.getValueFor(token().getName())))
    {
        NumberDescriptor *len = new NumberDescriptor(TypeDescriptor::INTEGER);
        if (id->type() == TypeDescriptor::INTEGER)
            len->value.intValue = id->valueInt.size();
        else
            len->value.intValue = id->valueString.size();
        return len;
    }
    else
    {
        std::cout << "Error! " << token().getName() << " does not return a ArrayDescriptor." << std::endl;
        exit(1);
    }
}

//CallNode
CallNode::CallNode(Token token, std::vector<ExprNode *> param) : ExprNode{token}
{
    parameters = param;
}

void CallNode::print()
{
    std::cout << token().getName() << "(";
    for (auto p : parameters)
    {
        p->print();
        std::cout << ", ";
    }
    std::cout << ")" << std::endl;
}

TypeDescriptor *CallNode::evaluate(SymTab &symTab)
{
    FunctionStatement *func = symTab.getFunctions()->findFunction(token().getName());
    symTab.openScope();
    for (int i = 0; i < parameters.size(); i++)
    {
        symTab.setValueFor(func->getParameterName(i), parameters[i]->evaluate(symTab));
    }
    func->evaluate(symTab);
    symTab.closeScope();
    return symTab.getReturnValue();
}