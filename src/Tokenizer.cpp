#include <iostream>
#include <string>
#include "Tokenizer.hpp"

// The function reads and returns all characters of the name.
std::string Tokenizer::readName()
{
    // This function is called when it is known that
    // the first character in input is an alphabetic character.

    std::string name;
    char c;
    while (inStream.get(c) && isalnum(c))
    {
        name += c;
    }
    if (inStream.good()) // In the loop, we have read one char too many.
        inStream.putback(c);
    return name;
}

// The function reads and returns all remaining digits.
int Tokenizer::readInteger()
{
    // This function is called when it is known that
    // the first character in input is a digit.

    int intValue = 0;
    char c;
    while (inStream.get(c) && isdigit(c))
    {
        intValue = intValue * 10 + c - '0';
    }
    if (inStream.good()) // In the loop, we have read one char too many.
        inStream.putback(c);
    return intValue;
}

Tokenizer::Tokenizer(std::ifstream &stream) : ungottenToken{false}, inStream{stream}, lastToken{}, parsingANewLine{true}, indentLevel{0}
{
    dedentTracker.push_back(0);
    indent = 0;
}

Token Tokenizer::getToken()
{

    if (ungottenToken)
    {
        ungottenToken = false;
        return lastToken;
    }

    char c;
    //int indent = 0;
    /*
    while( inStream.get(c) && isspace(c) && c != '\n' )  // Skip spaces but not new-line chars.
        ;
    */

    while (inStream.get(c) && c == ' ')
    {
        //while I am skipping white space on a new line I am keeping track of the number of spaces for the indent
        if (parsingANewLine)
        {
            if (c == ' ')
            {
                indent++;
            }
            else if (c == '\t')
            {
                indent += (indent % 8);
            }
        }
    }
    //skips rest of the line if there is a comment
    if (c == '#')
    {
        while (inStream.get(c) && c != '\n')
            ;
        //skips to the end of line EOL
    }
    if (inStream.bad())
    {
        std::cout << "Error while reading the input stream in Tokenizer.\n";
        exit(1);
    }

    Token token;
    //detecting indents and dedents
    //C is not EOL
    if (parsingANewLine && c != '\n')
    {
        parsingANewLine = false;
        //if the indent level has grown return a indent token
        if (indent > indentLevel)
        {
            inStream.putback(c);
            token.setIndent(indent);
            indentLevel = indent;
            dedentTracker.push_back(indent); //anytime we create indent we also create a dedent at the same level
            _tokens.push_back(token);
            return lastToken = token;
        }
        //if the indent level has shrunk return a dedent token
        else if (indent < indentLevel)
        {
            inStream.putback(c);
            token = createDedent();
            if (token.getDedent() > indent)
                parsingANewLine = true;
            return lastToken = token;
        }
    }
    if (c <= std::char_traits<char>::eof() || inStream.eof())
    {
        token.eof() = true;
    }
    else if (c == '\n' || c == ';')
    {
        parsingANewLine = true; //going to a new line so set this to true
        indent = 0;
        token.eol() = true;
    }
    else if (isdigit(c))
    { // a integer?
        // put the digit back into the input stream so
        // we read the entire number in a function
        inStream.putback(c);
        token.setWholeNumber(readInteger());
    }
    else if (c == '=')
    {
        if (inStream.peek() == '=')
        {
            std::string rel;
            rel.push_back(c);
            c = inStream.get();
            rel.push_back(c);
            token.setRelational(rel);
        }
        else
        {
            token.symbol(c);
        }
    }
    else if (c == '+' || c == '-' || c == '*' || c == '/' || c == '%')
    {
        token.symbol(c);
    }
    else if (c == ';' || c == ':')
    {
        token.symbol(c);
    }
    else if (c == '(' || c == ')')
    {
        token.symbol(c);
    }

    else if (c == '[' || c == ']')
    {
        token.symbol(c);
    }

    else if (c == '.')
    {
        token.symbol(c);
    }
    else if (c == '"')
    {
        std::string s = "";
        while (inStream.get(c) && c != '"')
        {
            s += c;
        }

        token.setString(s);
    }

    else if (c == '>' || c == '<' || c == '!')
    {
        std::string tempRel;
        if (inStream.peek() == '=')
        {
            tempRel.push_back(c);
            c = inStream.get();
        }
        tempRel.push_back(c);
        token.setRelational(tempRel);
    }
    else if (c == ',')
    {
        token.symbol(c);
    }
    else if (isalpha(c)) //alpha numerical a/A [0-9][a-Z]
    {                    // an identifier?
        // put c back into the stream so we can read the entire name in a function.
        inStream.putback(c);
        token.setName(readName());
    }
    else
    {
        std::cout << "Unknown character in input. ->" << c << "<-" << std::endl;
        exit(1);
    }
    _tokens.push_back(token);

    return lastToken = token;
}

void Tokenizer::ungetToken()
{
    ungottenToken = true;
}

std::vector<std::string> Tokenizer::printProcessedTokens()
{
    std::vector<std::string> ret;
    for (auto iter = _tokens.begin(); iter != _tokens.end(); ++iter)
    {
        std::string temp;
        temp += iter->print();
        std::cout << std::endl;
        temp += "\n";
        ret.push_back(temp);
    }
    return ret;
}

//grab the most recent dedent, delete it, and return a dedent token
Token Tokenizer::createDedent()
{
    Token tok;

    if (dedentTracker.size() > 0)
    {

        tok.setDedent(dedentTracker.back());

        indentLevel = dedentTracker[dedentTracker.size() - 2];
        dedentTracker.pop_back();
        _tokens.push_back(tok);
        return tok;
    }
    else
    {
        tok.setDedent(0);
        return tok;
    }
}