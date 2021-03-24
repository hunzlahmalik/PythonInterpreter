//
// Created by Ali A. Kooshesh on 1/30/19.
//

#include <iostream>
#include "Token.hpp"

Token::Token() : _name{""}, _eof{false}, _eol{false}, _symbol{'\0'}, _isWholeNumber{false}, _isDedent{false}, _isIndent{false} {}

std::string Token::print() const
{
    std::string out;
    if (eol())
       out+= "EOL\n";
    else if (eof())
       out+= "EOF";
    else if (isOpenParen())
       out+= "(";
    else if (isCloseParen())
       out+= ")";
    else if (isOpenBracket())
       out+= "[";
    else if (isCloseBracket())
       out+= "]";
    else if (isPeriod())
       out+= ".";
    else if (isAssignmentOperator())
       out+= " = ";
    else if (isSemiColon())
       out+= ";";
    else if (isColon())
       out+= ":";
    else if (isMultiplicationOperator())
       out+= " * ";
    else if (isAdditionOperator())
       out+= " + ";
    else if (isSubtractionOperator())
       out+= " - ";
    else if (isModuloOperator())
       out+= " % ";
    else if (isDivisionOperator())
       out+= " / ";
    else if (isName())
       out+= getName();
    else if (isWholeNumber())
       out+= getWholeNumber();
    else if (isRelational())
       out+= " " + _relational + " ";
    else if (isQuote())
       out+= "\"";
    else if (isString())
       out+= getString();
    else if (isComma())
       out+= " , ";
    else if (isDedent())
       out+= "DEDENT: " + std::to_string(getDedent());
    else if (isIndent())
       out+= "INDENT: " + std::to_string(getIndent());
    else
       out+= "Uninitialized token.\n";

    std::cout << out;
    return out;
}
