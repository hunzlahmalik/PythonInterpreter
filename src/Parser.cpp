
/*  The majority of the work is done by the class 'convert'.
    This class builds an expression tree using the input infix
    expression.  A post-order traversal of the expression tree 'dumps'
    it into an array in postfix form.  The iterator copies the token
    from this array to user's arrays.

*/

#include <vector>
#include <iostream>
#include <algorithm>

#include "Token.hpp"
#include "Parser.hpp"
#include "Statements.hpp"

// Parser functions

void Parser::die(std::string where, std::string message, Token &token)
{
    std::cout << where << " " << message << std::endl;
    token.print();
    std::cout << std::endl;
    std::cout << "\nThe following is a list of tokens that have been identified up to this point.\n";
    tokenizer.printProcessedTokens();
    exit(1);
}

Statements *Parser::statements()
{
    // This function is called when we KNOW that we are about to parse
    // a series of assignment statements.
    //modified to include relational statements

    Statements *stmts = new Statements();
    Token tok = tokenizer.getToken();

    int indentLevel; //this is used to see where the statements indent level is, the first one should be 0
    if (tok.isIndent())
    {                                  // if getToken returns an indent
        indentLevel = tok.getIndent(); // set the indentLevel to the indent token
        tok = tokenizer.getToken();    // get next token past indent
    }
    else
    {
        indentLevel = 0;
    }
    while (tok.isName() || tok.eol())
    {
        while (tok.eol())
        {
            tok = tokenizer.getToken();
        }
        if (tok.eof())
        {
            break;
        }
        //if we receive a dedent token that is at the correct level then we break out of the loop
        if (tok.isDedent())
        {
            if (tok.getDedent() != indentLevel)
            {
                die("Parser::statements dedent", "incorrect dedent", tok);
            }
            break;
        }
        if (tok.isPrint()) // if we see the keyword print
        {
            std::vector<ExprNode *> mess = testList();
            PrintStatement *printStatement = new PrintStatement(mess);
            stmts->addStatement(printStatement);
        }
        else if (tok.isLen())
        {
            tok = tokenizer.getToken();

            if (!tok.isOpenParen())
                die("Parser::len", "Expected a open parenthesis, instead got", tok);

            tok = tokenizer.getToken();

            if (!tok.isName())
                die("Parser::len", "Expected a name, instead got", tok);
        }
        else if (tok.isFor())
        {
            int loopIndent;
            Token id = tokenizer.getToken();

            if (!id.isName())
                die("Parser::forStatement", "Expected a name, instead got", tok);

            tok = tokenizer.getToken();

            if (!tok.isIn())
                die("Parser::forStatement", "Expected a in, instead got", tok);

            tok = tokenizer.getToken();

            if (!tok.isRange())
                die("Parser::forStatement", "Expected range, instead got", tok);

            tok = tokenizer.getToken();

            if (!tok.isOpenParen())
                die("Parser::forStatement", "Expected a open parenthesis, instead got", tok);
            std::vector<ExprNode *> range = testList();
            tok = tokenizer.getToken();

            if (!tok.isCloseParen())
            {
                die("Parser::forStatement", "Expected a close parenthesis, instead got", tok);
            }

            tok = tokenizer.getToken();
            if (!tok.isColon())
            {
                die("Parser::forStatement", "Expected a :, instead got", tok);
            }
            Statements *bod = func_body(); //body is a function that checks indent and dedent tokens, and returns statements

            ForStatement *forStatement = new ForStatement(id, range, bod);
            stmts->addStatement(forStatement);
        }
        else if (tok.isIf())
        {
            std::vector<IfNode *> ifs;
            ExprNode *rel;
            do
            {
                if (tok.isElse())
                {
                    tok = tokenizer.getToken();
                    if (!tok.isIf())
                    {
                        tokenizer.ungetToken();
                        rel = returnTrue();     //this is not an else if and should always execute if we get to it
                        tokenizer.ungetToken(); //mine
                    }
                    else
                    {
                        rel = test();
                    }
                }
                else
                {
                    rel = test();
                }
                tok = tokenizer.getToken();
                if (!tok.isColon())
                {
                    die("Parser::ifStatement", "Expected a :, instead got", tok);
                }

                Statements *bod = suite();

                IfNode *node = new IfNode(rel, bod);
                ifs.push_back(node);
                tok = tokenizer.getToken();

            } while (tok.isElse());
            IfStatement *ifStmt = new IfStatement(ifs);
            stmts->addStatement(ifStmt);
            tokenizer.ungetToken();
        }
        else if (tok.isDef())
        {
            Token funcName = tokenizer.getToken();
            functions.push_back(funcName.getName());
            if (!funcName.isName() || funcName.isKeyword())
                die("Parser::defStatement", "Expected a name, instead got", tok);

            tok = tokenizer.getToken();

            if (!tok.isOpenParen())
                die("Parser::defStatement", "Expected a open parenthesis, instead got", tok);

            std::vector<std::string> param = parameter_list();

            tok = tokenizer.getToken();

            if (!tok.isCloseParen())
                die("Parser::defStatement", "Expected a close parenthesis, instead got", tok);

            tok = tokenizer.getToken();

            if (!tok.isColon())
                die("Parser::defStatement", "Expected a colon, instead got", tok);

            Statements *body = suite();

            FunctionDef *funcState = new FunctionDef(param, body, funcName.getName());
            stmts->addStatement(funcState);
        }
        else if (std::find(functions.begin(), functions.end(), tok.getName()) != functions.end())
        {
            Token funcName = tok;
            tok = tokenizer.getToken();

            if (!tok.isOpenParen())
            {
                die("Parser::callStatement", "Expected a open parenthesis, instead got", tok);
            }

            std::vector<ExprNode *> body = testList();

            tok = tokenizer.getToken();

            if (!tok.isCloseParen())
                die("Parser::callStatement", "Expected a close parenthesis, instead got", tok);

            CallNode *call = new CallNode(funcName, body);
            FunctionCall *callStmt = new FunctionCall(call);
            stmts->addStatement(callStmt);
        }
        else if (tok.isReturn())
        {
            ExprNode *ret = test();
            ReturnStatement *retStmt = new ReturnStatement(ret);
            stmts->addStatement(retStmt);
            tok = tokenizer.getToken();
            while (tok.eol())
            {
                tok = tokenizer.getToken();
            }
            break;
        }
        else
        {

            tokenizer.ungetToken();
            Statement *assignStmt = assignStatement();
            stmts->addStatement(assignStmt);
        }
        tok = tokenizer.getToken();
    }

    if (tok.isIndent())
        die("Parser::Statements", "Expected a eof, instead got", tok);

    tokenizer.ungetToken();

    return stmts;
}

//body checks for an indent, creates a new set of statements at that indent level, and checks the dedent token
Statements *Parser::suite()
{
    Token tok;
    tok = tokenizer.getToken();
    if (!tok.eol())
    {
        die("Parser::body", "Expected a eol, instead got", tok);
    }
    while (tok.eol())
    {
        tok = tokenizer.getToken();
    }
    if (!tok.isIndent())
    {
        die("Parser::body", "Expected a indent, instead got", tok);
    }
    int ifLevel = tok.getIndent();

    tokenizer.ungetToken();
    Statements *bod = statements();
    tok = tokenizer.getToken();
    if (!tok.isDedent())
        die("Parser::body", "Expected a dedent, instead got", tok);
    if (tok.getDedent() != ifLevel)
    {
        std::cout << "Dedent is " << tok.getDedent() << std::endl;
        die("Parser::body", "incorrect indent level, expected " + std::to_string(ifLevel) + " got ",
            tok);
    }
    return bod;
}

Statements *Parser::func_body()
{
    //Token tok;
    //tok = tokenizer.getToken();
    /*
    if(!tok.eol()){
        die("Parser::body", "Expected a eol, instead got", tok);
    }
    while (tok.eol()) {
        tok = tokenizer.getToken();
    }
    if (!tok.isIndent()) {
        die("Parser::body", "Expected a indent, instead got", tok);
    }
    int ifLevel = tok.getIndent();

    tokenizer.ungetToken();
    //Statements *bod = func_suite();
    Statements *bod = suite();/*
    tok = tokenizer.getToken();
    if (!tok.isDedent())
        die("Parser::body", "Expected a dedent, instead got", tok);
    if (tok.getDedent() != ifLevel) {
        std::cout << "Dedent is " << tok.getDedent() << std::endl;
        die("Parser::body", "incorrect indent level, expected " + std::to_string(ifLevel) + " got ",
            tok);
    }*/
    return suite();
}

/*
Statements *Parser::func_suite() {
    Statements *stmts = new Statements();
    Token tok = tokenizer.getToken();

    int indentLevel;    //this is used to see where the statements indent level is, the first one should be 0
    if(tok.isIndent()) {                // if getToken returns an indent
        indentLevel = tok.getIndent();  // set the indentLevel to the indent token
        tok = tokenizer.getToken();     // get next token past indent
    }
    else{
        indentLevel = 0;
    }
    while (tok.isName() || tok.eol()) {
        while(tok.eol()){
            tok = tokenizer.getToken();
        }
        if(tok.eof()){
            break;
        }
        //if we receive a dedent token that is at the correct level then we break out of the loop
        if(tok.isDedent()){
            if(tok.getDedent() != indentLevel)
            {
                die("Parser::statements dedent", "incorrect dedent", tok);
            }
            break;

        }
        if(tok.isPrint()) // if we see the keyword print
        {
            std::vector<ExprNode*> mess = testList();
            PrintStatement *printStatement = new PrintStatement(mess);
            stmts->addStatement(printStatement);

        }
        else if(tok.isLen()){
            tok = tokenizer.getToken();

            if(!tok.isOpenParen())
                die("Parser::len", "Expected a open parenthesis, instead got", tok);

            tok = tokenizer.getToken();

            if(!tok.isName())
                die("Parser::len", "Expected a name, instead got", tok);
        }
        else if(tok.isFor()){
            int loopIndent;
            Token id = tokenizer.getToken();

            if(!id.isName())
                die("Parser::forStatement", "Expected a name, instead got", tok);

            tok = tokenizer.getToken();

            if(!tok.isIn())
                die("Parser::forStatement", "Expected a in, instead got", tok);

            tok = tokenizer.getToken();

            if(!tok.isRange())
                die("Parser::forStatement", "Expected range, instead got", tok);

            tok = tokenizer.getToken();

            if(!tok.isOpenParen())
                die("Parser::forStatement", "Expected a open parenthesis, instead got", tok);
            std::vector<ExprNode*> range = testList();
            tok = tokenizer.getToken();

            if(!tok.isCloseParen()){
                die("Parser::forStatement", "Expected a close parenthesis, instead got", tok);
            }

            tok = tokenizer.getToken();
            if(!tok.isColon()){
                die("Parser::forStatement", "Expected a :, instead got", tok);
            }
            Statements *bod = suite();   //body is a function that checks indent and dedent tokens, and returns statements

            ForStatement *forStatement = new ForStatement(id, range, bod);
            stmts->addStatement(forStatement);
        }
        else if(tok.isIf()){
            std::vector<IfNode*> ifs;
            ExprNode *rel;
            do {
                if(tok.isElse()) {
                    tok = tokenizer.getToken();
                    if (!tok.isIf()) {
                        tokenizer.ungetToken();
                        rel = returnTrue(); //this is not an else if and should always execute if we get to it
                    }
                    else{
                        rel = test();
                    }
                }
                else{
                    rel = test();
                }
                tok = tokenizer.getToken();
                if(!tok.isColon()){
                    die("Parser::ifStatement", "Expected a :, instead got", tok);
                }

                Statements *bod = suite();

                IfNode *node = new IfNode(rel, bod);
                ifs.push_back(node);
                tok = tokenizer.getToken();

            }while(tok.isElse());
            IfStatement *ifStmt = new IfStatement(ifs);
            stmts->addStatement(ifStmt);
            tokenizer.ungetToken();
        }
        else if(tok.isDef()){
            Token funcName = tokenizer.getToken();
            functions.push_back(funcName.getName());
            if(!funcName.isName() || funcName.isKeyword())
                die("Parser::defStatement", "Expected a name, instead got", tok);

            tok = tokenizer.getToken();

            if(!tok.isOpenParen())
                die("Parser::defStatement", "Expected a open parenthesis, instead got", tok);

            std::vector<std::string> param = parameter_list();

            tok = tokenizer.getToken();

            if(!tok.isCloseParen())
                die("Parser::defStatement", "Expected a close parenthesis, instead got", tok);

            tok = tokenizer.getToken();

            if(!tok.isColon())
                die("Parser::defStatement", "Expected a colon, instead got", tok);

            Statements *body = func_body();

            FunctionDef *funcState = new FunctionDef(param, body, funcName.getName());
            stmts->addStatement(funcState);
        }
        else if(std::find(functions.begin(), functions.end(), tok.getName()) != functions.end()){
            Token funcName = tok;
            tok = tokenizer.getToken();

            if(!tok.isOpenParen()){
                die("Parser::callStatement", "Expected a open parenthesis, instead got", tok);
            }

            std::vector<ExprNode*> body = testList();

            tok = tokenizer.getToken();

            if(!tok.isCloseParen())
                die("Parser::callStatement", "Expected a close parenthesis, instead got", tok);

            CallNode *call = new CallNode(funcName, body);
            FunctionCall *callStmt = new FunctionCall(call);
            stmts->addStatement(callStmt);
        }
        else {

            tokenizer.ungetToken();
            Statement *assignStmt = assignStatement();
            stmts->addStatement(assignStmt);
        }
        tok = tokenizer.getToken();

    }


    tokenizer.ungetToken();

    return stmts;
}*/

//creates a token that represents a true value and checks if the next token is a colon
ExprNode *Parser::returnTrue()
{
    Token tok;
    tok.setWholeNumber(1);
    tok = tokenizer.getToken();
    if (!tok.isColon())
        die("Parser::returnTrue", "Expected a :, instead got", tok);
    return new WholeNumber(tok);
}

std::vector<ExprNode *> Parser::testList()
{
    Token tok;
    tok = tokenizer.getToken();
    std::vector<ExprNode *> head;
    if (!tok.isCloseParen())
    {
        tokenizer.ungetToken();
        head.push_back(test());
        tok = tokenizer.getToken();
        while (tok.isComma())
        {
            head.push_back(test());
            tok = tokenizer.getToken();
        }
    }
    tokenizer.ungetToken();
    return head;
}

ExprNode *Parser::test()
{
    //<test> -> <or_test>
    ExprNode *result = or_test();
    //Token tok = tokenizer.getToken();
    /*
    if(!tok.isColon()){
        die("Parser::test", "Expected a :, instead got", tok);
    }
     */
    return result;
}

ExprNode *Parser::or_test()
{
    //<or_test> -> <and_test> {'or' <and_test>}
    ExprNode *left = and_test();
    Token tok = tokenizer.getToken();
    while (tok.isOr())
    {
        InfixExprNode *p = new InfixExprNode(tok);
        p->left() = left;
        p->right() = and_test();
        left = p;
        tok = tokenizer.getToken();
    }
    tokenizer.ungetToken();
    return left;
}

ExprNode *Parser::and_test()
{
    //<and_test> -> <not_test> {'and' <not_test>}
    ExprNode *left = not_test();
    Token tok = tokenizer.getToken();
    while (tok.isAnd())
    {
        InfixExprNode *p = new InfixExprNode(tok);
        p->left() = left;
        p->right() = not_test();
        left = p;
        tok = tokenizer.getToken();
    }
    tokenizer.ungetToken();
    return left;
}

ExprNode *Parser::not_test()
{
    //<not_test> -> 'not' <not_test> | <relTerm>

    Token tok = tokenizer.getToken();
    if (tok.isNot())
    {
        NotNode *p = new NotNode(tok);

        p->rel() = not_test();
        return p;
    }
    tokenizer.ungetToken();
    return relTerm();
}

ExprNode *Parser::relTerm()
{
    //<relTerm> -> <expr> {(>, <, >=, <=) <expr>}
    ExprNode *left = expr();
    Token tok = tokenizer.getToken();
    //while(tok.isGreaterThanSign() || tok.isLessThanSign() || tok.isGreaterEqualThanSign() || tok.isLessThanEqualSign())
    while (tok.isRelational())
    {

        InfixExprNode *p = new InfixExprNode(tok);
        p->left() = left;
        p->right() = expr();
        left = p;
        tok = tokenizer.getToken();
    }
    tokenizer.ungetToken();
    return left;
}

Statement *Parser::assignStatement()
{
    Token varName = tokenizer.getToken();
    if (!varName.isName())
        die("Parser::assignStatement", "Expected a name token, instead got", varName);
    if (varName.isKeyword())
        die("Parser::assignStatement", "Cannot use keyword ", varName);

    Token assignOp = tokenizer.getToken();

    if (assignOp.isPeriod())
    {
        return array_op(varName);
    }
    else if (assignOp.isOpenBracket())
    {
        Token index = tokenizer.getToken();
        Token tok = tokenizer.getToken();
        if (!tok.isCloseBracket())
            die("Parser::assignStatement", "Expected a close bracket, instead got", assignOp);

        assignOp = tokenizer.getToken();
        if (!assignOp.isAssignmentOperator())
            die("Parser::assignStatement", "Expected an equal sign, instead got", assignOp);

        ExprNode *rightHandSideExpr = expr();

        tok = tokenizer.getToken();
        if (!tok.eol())
            die("Parser::assignStatement", "Expected a eol, instead got", tok);

        return new ArrAccessAssign(varName.getName(), index.getWholeNumber(), rightHandSideExpr);
    }
    if (!assignOp.isAssignmentOperator())
        die("Parser::assignStatement", "Expected an equal sign, instead got", assignOp);

    // We test for encountering an array initialization by getting the next token
    Token arrayTest = tokenizer.getToken(); // grab next token
    if (arrayTest.isOpenBracket())
    { // is it [ ?
        // array initialization             // if so, initialize an array
        // return something?????????
        arrayTest = tokenizer.getToken();
        if (arrayTest.isCloseBracket())
        {
            std::vector<ExprNode *> rhsArr;
            return new ArrAssignmentStatement(varName.getName(), rhsArr);
        }
        tokenizer.ungetToken();
        std::vector<ExprNode *> rhsArr = testList();
        tokenizer.ungetToken();
        Token closed = tokenizer.getToken();
        if (!closed.isCloseBracket())
            die("Parser::assignStatement", "Expected a closed bracket, instead got", closed);
        ArrAssignmentStatement *a = new ArrAssignmentStatement(varName.getName(), rhsArr);
        return a;
    }
    // if we grab the token and it is NOT [, we have a regular assignment expression
    else
    {
        tokenizer.ungetToken(); // unget the token
    }

    // we only ever hit this line IF we have a regular assignment expression, ie "a = 7"
    ExprNode *rightHandSideExpr = expr();

    Token tok = tokenizer.getToken();
    if (!tok.eol())
        die("Parser::assignStatement", "Expected a eol, instead got", tok);

    return new AssignmentStatement(varName.getName(), rightHandSideExpr);
}

ExprNode *Parser::expr()
{
    // This function parses the grammar rules:

    // <expr> -> <term> { <add_op> <term> }
    // <add_op> -> + | -

    // However, it makes the <add_op> left associative.

    ExprNode *left = term();
    Token tok = tokenizer.getToken();
    while (tok.isAdditionOperator() || tok.isSubtractionOperator())
    {
        InfixExprNode *p = new InfixExprNode(tok);
        p->left() = left;
        p->right() = term();
        left = p;
        tok = tokenizer.getToken();
    }
    tokenizer.ungetToken();
    return left;
}

ExprNode *Parser::term()
{
    // This function parses the grammar rules:

    // <term> -> <primary> { <mult_op> <primary> }
    // <mult_op> -> * | / | %

    // However, the implementation makes the <mult-op> left associate.
    ExprNode *left = primary();
    Token tok = tokenizer.getToken();

    while (tok.isMultiplicationOperator() || tok.isDivisionOperator() || tok.isModuloOperator())
    {
        InfixExprNode *p = new InfixExprNode(tok);
        p->left() = left;
        p->right() = primary();
        left = p;
        tok = tokenizer.getToken();
    }
    tokenizer.ungetToken();
    return left;
}

ExprNode *Parser::primary()
{
    // This function parses the grammar rules:

    // <primary> -> [0-9]+
    // <primary> -> [_a-zA-Z]+
    // <primary> -> (<expr>)

    Token tok = tokenizer.getToken();
    if (tok.isSubtractionOperator())
    {
        tok = tokenizer.getToken();
        if (tok.isWholeNumber())
        {
            tok.setWholeNumber(-tok.getWholeNumber());
            return new WholeNumber(tok);
        }
        else
        {
            die("Parser::primary", "Expected a WholeNumber, instead got", tok);
        }
    }
    if (tok.isWholeNumber())
        return new WholeNumber(tok);
    else if (tok.isLen())
    {
        return array_len();
    }
    else if (tok.isName())
    {
        Token bracket = tokenizer.getToken();
        if (bracket.isOpenBracket())
        {
            ExprNode *tested = test();
            bracket = tokenizer.getToken();
            if (!bracket.isCloseBracket())
                die("Parser::primary", "Expected a closeBracket, instead got", tok);
            return new ArrayNode(tok, tested);
        }
        else if (std::find(functions.begin(), functions.end(), tok.getName()) != functions.end())
        {

            std::vector<ExprNode *> param = testList();
            Token paren = tokenizer.getToken();
            if (!paren.isCloseParen())
                die("Parser::primary", "Expected a close parenthesis, instead got", tok);
            return new CallNode(tok, param);
        }

        tokenizer.ungetToken();
        return new Variable(tok);
    }
    else if (tok.isOpenParen())
    {
        ExprNode *p = expr();
        Token token = tokenizer.getToken();
        if (!token.isCloseParen())
            die("Parser::primary", "Expected close-parenthesis, instead got", token);
        return p;
    }
    else if (tok.isString())
    {
        return new StringNode(tok);
    }
    die("Parser::primary", "Unexpected token", tok);

    return nullptr; // Will not reach this statement!
}

ExprNode *Parser::array_len()
{
    Token paren = tokenizer.getToken();
    if (paren.isOpenParen())
    {
        Token id = tokenizer.getToken();
        if (!id.isName())
            die("Parser::array_len", "Expected a name, instead got", paren);
        paren = tokenizer.getToken();
        if (!id.isName())
            die("Parser::array_len", "Expected a CloseParen, instead got", paren);

        return new ArrayLen(id);
    }
    else
    {
        die("Parser::array_len", "Expected a openParen, instead got", paren);
    }
}

Statement *Parser::array_op(Token varName)
{
    Token arrayOp = tokenizer.getToken();
    if (arrayOp.isPop())
    {
        Token paren = tokenizer.getToken();
        if (!paren.isOpenParen())
            die("Parser::array_op", "Expected a open parenthesis, instead got", varName);
        paren = tokenizer.getToken();
        if (!paren.isCloseParen())
            die("Parser::array_op", "Expected a close parenthesis, instead got", varName);
        return new ArrOpPop(varName.getName());
    }
    else if (arrayOp.isAppend())
    {
        Token paren = tokenizer.getToken();
        if (!paren.isOpenParen())
            die("Parser::array_op", "Expected a open parenthesis, instead got", varName);
        ExprNode *index = test();
        paren = tokenizer.getToken();
        if (!paren.isCloseParen())
            die("Parser::array_op", "Expected a close parenthesis, instead got", varName);
        return new ArrOpAppend(varName.getName(), index);
    }
    else
    {
        die("Parser::array_op", "Expected a array op, instead got", varName);
    }
}

std::vector<std::string> Parser::parameter_list()
{
    std::vector<std::string> param;
    Token tok = tokenizer.getToken();
    while (tok.isName())
    {
        param.push_back(tok.getName());
        tok = tokenizer.getToken();
        if (!tok.isComma())
            break;
        tok = tokenizer.getToken();
    }
    tokenizer.ungetToken();

    return param;
}
