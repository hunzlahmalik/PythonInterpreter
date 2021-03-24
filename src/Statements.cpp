//
// Created by Ali A. Kooshesh on 2/5/19.
//

#include "Statements.hpp"
#include "SymTab.hpp"

// Statement
Statement::Statement() {}

Statement::~Statement() {}

// Statements

Statements::Statements() {}

Statements::~Statements()
{
    for (auto s : _statements)
        delete s;
}

void Statements::addStatement(Statement *statement) { _statements.push_back(statement); }

void Statements::print()
{
    for (auto s : _statements)
        s->print();
}

void Statements::evaluate(SymTab &symTab)
{
    for (auto s : _statements)
    {
        s->evaluate(symTab);
    }
}

// AssignmentStatement

AssignmentStatement::AssignmentStatement() : _lhsVariable{""}, _rhsExpression{nullptr} {}

AssignmentStatement::~AssignmentStatement()
{
    delete _rhsExpression;
}

AssignmentStatement::AssignmentStatement(std::string lhsVar, ExprNode *rhsExpr) : _lhsVariable{lhsVar}, _rhsExpression{rhsExpr} {}

void AssignmentStatement::evaluate(SymTab &symTab)
{
    symTab.setValueFor(lhsVariable(), _rhsExpression->evaluate(symTab));
}

std::string &AssignmentStatement::lhsVariable()
{
    return _lhsVariable;
}

ExprNode *&AssignmentStatement::rhsExpression()
{
    return _rhsExpression;
}

void AssignmentStatement::print()
{
    std::cout << _lhsVariable << " = ";
    _rhsExpression->print();
    std::cout << std::endl;
}

// Array assignment statement
ArrAssignmentStatement::ArrAssignmentStatement() : _lhsVariable{""}, _rhsArray{} {}

ArrAssignmentStatement::~ArrAssignmentStatement()
{
    for (int i = 0; i < _rhsArray.size(); i++)
    {
        delete _rhsArray[i]; // clear memory
    }
}

ArrAssignmentStatement::ArrAssignmentStatement(std::string lhsVar, std::vector<ExprNode *> rhsArray)
{
    _lhsVariable = lhsVar;
    //std::vector<ExprNode*> temp;
    for (int i = 0; i < rhsArray.size(); i++)
    {
        _rhsArray.push_back(rhsArray[i]);
    }
}

void ArrAssignmentStatement::evaluate(SymTab &symTab)
{
    //symTab.setValueFor(lhsVariable(), _rhsArray->evaluate(symTab));
    bool isInt = false;
    if (_rhsArray.size() == 0)
    {
        symTab.setValueFor(_lhsVariable, new ArrayDescriptor(TypeDescriptor::INTEGER));
        return;
    }
    TypeDescriptor *element = _rhsArray[0]->evaluate(symTab); // first, we test the first element to see its type
    ArrayDescriptor *temp;
    if (element->type() == TypeDescriptor::INTEGER)
    { // if it is of type INTEGER
        isInt = true;
        temp = new ArrayDescriptor(TypeDescriptor::INTEGER);                                 // creates new ArrayDescriptor with int types (empty for now)
        temp->valueInt.push_back(dynamic_cast<NumberDescriptor *>(element)->value.intValue); // we need to dynamically cast to get NumberDescriptor values
                                                                                             // (instead of TypeDescriptor)
    }
    else
    {                                                                                  // we have an array of strings
        temp = new ArrayDescriptor(TypeDescriptor::STRING);                            //set up new ArrayDescriptor
        temp->valueString.push_back(dynamic_cast<StringDescriptor *>(element)->value); // dynamically cast to StringDescriptor to get value
    }

    // now that we know the type of the array, we grab each value of the array
    for (int i = 1; i < _rhsArray.size(); i++)
    {
        element = _rhsArray[i]->evaluate(symTab);

        if (element->type() == TypeDescriptor::INTEGER)
        { // if it is of type INTEGER
            if (!isInt)
            {
                std::cout << "ArrAssignmentStatement::evaluate -- Error: Non-homogenous typed array\n";
                exit(1);
            }
            temp->valueInt.push_back(dynamic_cast<NumberDescriptor *>(element)->value.intValue); // we need to dynamically cast to get NumberDescriptor values
            // (instead of TypeDescriptor)
        }
        else
        { // we have an array of strings
            if (isInt)
            {
                std::cout << "ArrAssignmentStatement::evaluate -- Error: Non-homogenous typed array\n";
                exit(1);
            }
            temp->valueString.push_back(dynamic_cast<StringDescriptor *>(element)->value); // dynamically cast to StringDescriptor to get value
        }
    }

    symTab.setValueFor(_lhsVariable, temp); //
}

std::string &ArrAssignmentStatement::lhsVariable()
{
    return _lhsVariable;
}

std::vector<ExprNode *> &ArrAssignmentStatement::rhsArray()
{
    return _rhsArray;
}

void ArrAssignmentStatement::print()
{
    std::cout << _lhsVariable << " = [";
    for (int i = 0; i < _rhsArray.size(); i++)
    {
        _rhsArray[i]->print();
        std::cout << ", ";
    }
    std::cout << "]";
    std::cout << std::endl;
}

//Assign to array index
ArrAccessAssign::ArrAccessAssign()
{
    _rhsExpression = nullptr;
}

ArrAccessAssign::~ArrAccessAssign()
{
    delete _rhsExpression;
}

ArrAccessAssign::ArrAccessAssign(std::string lhs, int i, ExprNode *rhs)
{
    _lhsVariable = lhs;
    index = i;
    _rhsExpression = rhs;
}

void ArrAccessAssign::evaluate(SymTab &symTab)
{
    ArrayDescriptor *arr = dynamic_cast<ArrayDescriptor *>(symTab.getValueFor(_lhsVariable));
    TypeDescriptor *r = _rhsExpression->evaluate(symTab);

    if (arr->type() == TypeDescriptor::INTEGER)
    {
        if (r->type() == TypeDescriptor::INTEGER)
        {
            arr->valueInt[index] = dynamic_cast<NumberDescriptor *>(r)->value.intValue;
        }
        else
        {
            std::cout << "Error! Cannot assign string to array of integers." << std::endl;
            exit(1);
        }
    }
    else
    {
        if (r->type() == TypeDescriptor::STRING)
        {
            arr->valueString[index] = dynamic_cast<StringDescriptor *>(r)->value;
        }
        else
        {
            std::cout << "Error! Cannot assign integer to array of strings." << std::endl;
            exit(1);
        }
    }
    symTab.setValueFor(_lhsVariable, arr);
}

void ArrAccessAssign::print()
{
    std::cout << _lhsVariable << "[" << index << "] = ";
    _rhsExpression->print();
    std::cout << std::endl;
}

// Array Pop Statement
ArrOpPop::ArrOpPop() : _arrName{""} {}

ArrOpPop::ArrOpPop(std::string arrName)
{
    _arrName = arrName;
}

void ArrOpPop::evaluate(SymTab &symTab)
{
    ArrayDescriptor *temp = dynamic_cast<ArrayDescriptor *>(symTab.getValueFor(_arrName));
    if (temp->type() == TypeDescriptor::INTEGER)
    {
        temp->valueInt.pop_back();
    }
    else
    {
        temp->valueString.pop_back();
    }
}

void ArrOpPop::print()
{
    std::cout << _arrName << ".pop() " << std::endl;
}

// Array Append Statement
ArrOpAppend::ArrOpAppend() : _arrName{""}, _test{nullptr} {}

ArrOpAppend::~ArrOpAppend()
{
    delete _test;
}

ArrOpAppend::ArrOpAppend(std::string arrName, ExprNode *test)
{
    _arrName = arrName;
    _test = test;
}

void ArrOpAppend::evaluate(SymTab &symTab)
{
    ArrayDescriptor *temp = dynamic_cast<ArrayDescriptor *>(symTab.getValueFor(_arrName));
    if (temp->type() == TypeDescriptor::INTEGER)
    {
        NumberDescriptor *appValue = dynamic_cast<NumberDescriptor *>(_test->evaluate(symTab));
        temp->valueInt.push_back(appValue->value.intValue);
    }
    else
    {
        StringDescriptor *appValue = dynamic_cast<StringDescriptor *>(_test->evaluate(symTab));
        temp->valueString.push_back(appValue->value);
    }
}

void ArrOpAppend::print()
{
    std::cout << _arrName << ".append( ";
    _test->print();
    std::cout << ")" << std::endl;
}

//Print Statement
PrintStatement::PrintStatement() : message{} {}

PrintStatement::~PrintStatement()
{
    for (auto a : message)
    {
        delete a;
    }
}

PrintStatement::PrintStatement(std::vector<ExprNode *> mess)
{
    for (unsigned i = 0; i < mess.size(); i++)
    {
        message.push_back(mess[i]);
    }
}

//Fixed it!!!!!!!!!!!!!!!!
void PrintStatement::evaluate(SymTab &symTab)
{
    for (int i = 0; i < message.size(); i++)
    {
        TypeDescriptor *temp = message[i]->evaluate(symTab);
        if (dynamic_cast<ArrayDescriptor *>(temp))
        {
            ArrayDescriptor *a = dynamic_cast<ArrayDescriptor *>(temp);
            if (a->type() == TypeDescriptor::INTEGER)
            {
                std::cout << "[";
                for (int i = 0; i < a->valueInt.size(); i++)
                {
                    std::cout << a->valueInt[i];
                    if (i != a->valueInt.size() - 1)
                        std::cout << ", ";
                }
                std::cout << "]";// << std::endl;
            }
            else
            {
                std::cout << "[";
                for (int i = 0; i < a->valueString.size(); i++)
                {
                    std::cout << "'" << a->valueString[i] << "'";
                    if (i != a->valueString.size() - 1)
                        std::cout << ", ";
                }
                std::cout << "]" << std::endl;
            }
        }
        else if (temp->type() == TypeDescriptor::INTEGER)
        {
            std::cout << dynamic_cast<NumberDescriptor *>(temp)->value.intValue;
        }
        else if (temp->type() == TypeDescriptor::STRING)
        {
            std::cout << dynamic_cast<StringDescriptor *>(temp)->value;
        }
        else
        {
            std::cout << "Found Nothing" << std::endl;
        }
    }
    std::cout << std::endl;
}

void PrintStatement::print()
{
    std::cout << "print ";
    for (unsigned i = 0; i < message.size(); i++)
    {
        message[i]->print();
    }
    std::cout << std::endl;
}

//for statement
ForStatement::ForStatement() : body{nullptr}
{
    initLow = false;
    initUpper = false;
    initIncrement = false;
}

ForStatement::~ForStatement()
{
    delete body;
    for (auto a : _range)
    {
        delete a;
    }
}
//three different constructors depending on how many numbers were given to range
ForStatement::ForStatement(Token tok, std::vector<ExprNode *> range, Statements *b)
{
    _range = range;
    body = b;
    name = tok;
    initLow = false;
    initUpper = false;
    initIncrement = false;
}

void ForStatement::evaluate(SymTab &symTab)
{

    if (_range.size() == 1)
    {
        initUpper = true;
        upper = dynamic_cast<NumberDescriptor *>(_range[0]->evaluate(symTab))->value.intValue;
        lower = 0;
    }
    else if (_range.size() == 2)
    {
        initLow = true;
        lower = dynamic_cast<NumberDescriptor *>(_range[0]->evaluate(symTab))->value.intValue;
        initUpper = true;
        upper = dynamic_cast<NumberDescriptor *>(_range[1]->evaluate(symTab))->value.intValue;
    }
    else if (_range.size() == 3)
    {
        initLow = true;
        lower = dynamic_cast<NumberDescriptor *>(_range[0]->evaluate(symTab))->value.intValue;
        initUpper = true;
        upper = dynamic_cast<NumberDescriptor *>(_range[1]->evaluate(symTab))->value.intValue;
        initIncrement = true;
        increment = dynamic_cast<NumberDescriptor *>(_range[2]->evaluate(symTab))->value.intValue;
    }
    else
    {
        std::cout << "Error! Invalid entry for range" << std::endl;
        exit(1);
    }
    NumberDescriptor *typeDesc = new NumberDescriptor(TypeDescriptor::INTEGER);
    typeDesc->value.intValue = lower;
    symTab.setValueFor(name.getName(), typeDesc);
    while (range())
    {
        body->evaluate(symTab);
        typeDesc = new NumberDescriptor(TypeDescriptor::INTEGER);
        typeDesc->value.intValue = lower;
        symTab.setValueFor(name.getName(), typeDesc);
    }
}

void ForStatement::print()
{
    if (initLow)
    {
        std::cout << "for " << name.getName() << " in Range(" << lower << ", " << upper;
        if (initIncrement)
        {
            std::cout << ", " << increment;
        }
    }
    else
    {
        std::cout << "for " << name.getName() << " in Range(" << upper;
    }

    std::cout << "):" << std::endl;
    body->print();
}

bool ForStatement::range()
{
    bool rng = lower < upper; //check whether range should return true or false, and then lower is incremented
    if (initIncrement)
    {
        lower += increment;
    }
    else
    {
        lower += 1;
    }
    return rng;
}

IfNode::IfNode() : relNode{nullptr}, body{nullptr}, evaluatedTrue{false} {}

IfNode::~IfNode()
{
    delete relNode;
    delete body;
}

IfNode::IfNode(ExprNode *rel, Statements *bod) 
    : relNode{rel}, body{bod}, evaluatedTrue{false} {}

void IfNode::print()
{
    std::cout << "if ";
    relNode->print();
    std::cout << " :" << std::endl;
    body->print();
}

void IfNode::evaluate(SymTab &symTab)
{
    if (dynamic_cast<NumberDescriptor *>(relNode->evaluate(symTab))->value.boolValue)
    {
        body->evaluate(symTab);
        evaluatedTrue = true; //This tells ifStatement that this if statement was evaluated and it doesn't need to check any more else ifs or elses
    }
}

bool IfNode::evaluated()
{
    bool eval = evaluatedTrue;
    evaluatedTrue = false; //if the if statement is in a loop evaluatedTrue needs to be reset to false so that ifStatement will evaluate properly
    return eval;
}

IfStatement::IfStatement() : body{nullptr} {}

IfStatement::~IfStatement()
{
    for (auto a : body)
    {
        delete a;
    }
}

IfStatement::IfStatement(std::vector<IfNode *> ifs) : body{ifs} {}

void IfStatement::print()
{
    for (unsigned i = 0; i < body.size(); i++)
        body[i]->print();
}

void IfStatement::evaluate(SymTab &symTab)
{
    for (unsigned i = 0; i < body.size(); i++)
    {
        body[i]->evaluate(symTab);
        if (body[i]->evaluated())
            break; //if we evaluated a ifNode break out of the loop
    }
}

//function statment implementation
FunctionStatement::FunctionStatement()
{
    suite = nullptr;
}

FunctionStatement::FunctionStatement(std::vector<std::string> param, Statements *s, std::string name)
{
    parameters = param;
    suite = s;
    _name = name;
}

FunctionStatement::~FunctionStatement()
{
    delete suite;
}

void FunctionStatement::print()
{
}

void FunctionStatement::evaluate(SymTab &symTab)
{
    suite->evaluate(symTab);
}

std::string FunctionStatement::getParameterName(int index)
{
    return parameters[index];
}

//FunctionDef
FunctionDef::FunctionDef()
{
    suite = nullptr;
}

FunctionDef::FunctionDef(std::vector<std::string> param, Statements *s, std::string name)
{
    parameters = param;
    suite = s;
    _name = name;
}

FunctionDef::~FunctionDef()
{
    delete suite;
}

void FunctionDef::print()
{
}

void FunctionDef::evaluate(SymTab &symTab)
{
    FunctionStatement *tempStatement = new FunctionStatement(parameters, suite, _name);

    symTab.getFunctions()->addFunction(_name, tempStatement);
}

Functions::Functions() {}

Functions::~Functions()
{
    for (auto func : functions)
    {
        delete func.second;
    }
}
void Functions::addFunction(std::string name, FunctionStatement *func)
{
    functions[name] = func;
}

bool Functions::isDefined(std::string name)
{
    return functions.find(name) != functions.end();
}
FunctionStatement *Functions::findFunction(std::string funcName)
{
    if (!isDefined(funcName))
    {
        std::cout << "Error! undefined function " << funcName << std::endl;
        exit(1);
    }
    return functions[funcName];
}

FunctionCall::FunctionCall(ExprNode *call)
{
    _call = call;
}

FunctionCall::~FunctionCall()
{
    delete _call;
}

void FunctionCall::print()
{
    _call->print();
}

void FunctionCall::evaluate(SymTab &symTab)
{
    _call->evaluate(symTab);
}

ReturnStatement::ReturnStatement(ExprNode *ret)
{
    _return = ret;
}

ReturnStatement::~ReturnStatement()
{
    delete _return;
}

void ReturnStatement::print()
{
    std::cout << "Return ";
    _return->print();
}

void ReturnStatement::evaluate(SymTab &symTab)
{
    symTab.storeReturnValue(_return->evaluate(symTab));
}