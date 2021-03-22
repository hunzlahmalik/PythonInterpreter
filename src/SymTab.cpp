//
// Created by Ali A. Kooshesh on 2/5/19.
//

#include <iostream>
#include "SymTab.hpp"
#include "Statements.hpp"

extern bool debug;

SymTab::SymTab()
{
    std::map<std::string, TypeDescriptor *> temp;
    symTab.push_back(temp);
    _func = new Functions();
}

SymTab::~SymTab()
{
    for (auto a : symTab)
    {
        for (auto b : a)
        {
            delete b.second;
        }
        a.clear();
    }
}

void SymTab::setValueFor(std::string vName, TypeDescriptor *value)
{
    // Define a variable by setting its initial value.
    if (debug)
    {
        std::cout << vName << " <- ";
        if (value->type() == TypeDescriptor::INTEGER)
        {
            std::cout << dynamic_cast<NumberDescriptor *>(value)->value.intValue << std::endl;
        }
        else if (value->type() == TypeDescriptor::STRING)
        {
            std::cout << dynamic_cast<StringDescriptor *>(value)->value << std::endl;
        }
    }

    symTab.back()[vName] = value;
}

bool SymTab::isDefined(std::string vName)
{
    return symTab.back().find(vName) != symTab.back().end() || symTab[0].find(vName) != symTab[0].end();
}

bool SymTab::isDefinedOwnScope(std::string vName)
{
    return symTab.back().find(vName) != symTab.back().end();
}

TypeDescriptor *SymTab::getValueFor(std::string vName)
{
    if (!isDefined(vName))
    {
        if (debug)
            std::cout << "SymTab::getValueFor: " << vName << " has not been defined.\n";
        exit(1);
    }
    TypeDescriptor *val;
    if (isDefinedOwnScope(vName))
        val = symTab.back().find(vName)->second;
    else
        val = symTab[0].find(vName)->second;
    if (debug)
    {
        if (val->type() == TypeDescriptor::INTEGER)
            std::cout << "SymTab::getValueFor: " << vName << " contains " << dynamic_cast<NumberDescriptor *>(val)->value.intValue << std::endl;
        else if (val->type() == TypeDescriptor::STRING)
            std::cout << "SymTab::getValueFor: " << vName << " contains " << dynamic_cast<StringDescriptor *>(val)->value << std::endl;
    }
    return val;
}

void SymTab::openScope()
{
    std::map<std::string, TypeDescriptor *> temp;
    symTab.push_back(temp);
}

void SymTab::closeScope()
{
    for (auto a : symTab.back())
    {
        if (!dynamic_cast<ArrayDescriptor *>(a.second))
            delete a.second;
    }
    symTab.pop_back();
}

void SymTab::storeReturnValue(TypeDescriptor *val)
{
    _return = val;
}

TypeDescriptor *SymTab::getReturnValue()
{
    return _return;
}
