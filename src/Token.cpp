//
// Created by Ali A. Kooshesh on 1/30/19.
//

#include <iostream>
#include "Token.hpp"

Token::Token() : _name{""}, _eof{false}, _eol{false}, _symbol{'\0'}, _isWholeNumber{false}, _isDedent{false}, _isIndent{false} {}

void Token::print() const
{
    if (eol())
        std::cout << "EOL\n";
    else if (eof())
        std::cout << "EOF";
    else if (isOpenParen())
        std::cout << "(";
    else if (isCloseParen())
        std::cout << ")";
    else if (isOpenBracket())
        std::cout << "[";
    else if (isCloseBracket())
        std::cout << "]";
    else if (isPeriod())
        std::cout << ".";
    else if (isAssignmentOperator())
        std::cout << " = ";
    else if (isSemiColon())
        std::cout << ";";
    else if (isColon())
        std::cout << ":";
    else if (isMultiplicationOperator())
        std::cout << " * ";
    else if (isAdditionOperator())
        std::cout << " + ";
    else if (isSubtractionOperator())
        std::cout << " - ";
    else if (isModuloOperator())
        std::cout << " % ";
    else if (isDivisionOperator())
        std::cout << " / ";
    else if (isName())
        std::cout << getName();
    else if (isWholeNumber())
        std::cout << getWholeNumber();
    else if (isRelational())
        std::cout << " " << _relational << " ";
    else if (isQuote())
        std::cout << "\"";
    else if (isString())
        std::cout << getString();
    else if (isComma())
        std::cout << " , ";
    else if (isDedent())
        std::cout << "DEDENT: " << getDedent();
    else if (isIndent())
        std::cout << "INDENT: " << getIndent();
    else
        std::cout << "Uninitialized token.\n";
}
