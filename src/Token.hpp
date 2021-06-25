#ifndef EXPRINTER_TOKEN_HPP
#define EXPRINTER_TOKEN_HPP
#include <string>

//tokens handler and tokens checker
class Token
{

public:
  Token();

  bool &eof() { return _eof; }
  bool &eol() { return _eol; }

  bool eof() const { return _eof; }
  bool eol() const { return _eol; }

  bool isOpenParen() const { return _symbol == '('; }
  bool isCloseParen() const { return _symbol == ')'; }

  bool isOpenBracket() const { return _symbol == '['; }
  bool isCloseBracket() const { return _symbol == ']'; }

  bool isQuote() const { return _symbol == '"'; }

  bool isPeriod() const { return _symbol == '.'; }

  void symbol(char c) { _symbol = c; }
  char symbol() { return _symbol; }

  bool isSemiColon() const { return _symbol == ';'; }
  bool isColon() const { return _symbol == ':'; }
  bool isAssignmentOperator() const { return _symbol == '='; }
  bool isMultiplicationOperator() const { return _symbol == '*'; }
  bool isAdditionOperator() const { return _symbol == '+'; }
  bool isSubtractionOperator() const { return _symbol == '-'; }
  bool isModuloOperator() const { return _symbol == '%'; }
  bool isDivisionOperator() const { return _symbol == '/'; }
  bool isArithmeticOperator() const
  {
    return isMultiplicationOperator() ||
           isAdditionOperator() ||
           isSubtractionOperator() ||
           isModuloOperator() ||
           isDivisionOperator();
  }

  bool isRelational() const { return _relational.length() > 0; }
  std::string getRelational() { return _relational; }
  void setRelational(std::string n) { _relational = n; }

  bool isEqualSign() const { return _relational == "=="; }
  bool isNotEqualSign() const { return _relational == "!="; }
  bool isGreaterThanSign() const { return _relational == ">"; }
  bool isLessThanSign() const { return _relational == "<"; }
  bool isGreaterEqualThanSign() const { return _relational == ">="; }
  bool isLessThanEqualSign() const { return _relational == "<="; }
  bool isOr() const { return _name == "or"; }
  bool isAnd() const { return _name == "and"; }
  bool isNot() const { return _name == "not"; }

  bool isKeyword() const { return isPrint() || isFor() || isIn() || isRange() || isIf() || isElse() || isLen() || isPop() || isAppend() || isDef() || isReturn(); }

  bool isPrint() const { return _name == "print"; }
  bool isFor() const { return _name == "for"; }
  bool isIn() const { return _name == "in"; }
  bool isRange() const { return _name == "range"; }
  bool isIf() const { return _name == "if"; }
  bool isElse() const { return _name == "else"; }
  bool isLen() const { return _name == "len"; }
  bool isPop() const { return _name == "pop"; }
  bool isAppend() const { return _name == "append"; }
  bool isDef() const { return _name == "def"; }
  bool isReturn() const { return _name == "return"; }
  bool isLambda() const { return _name == "lambda"; }

  bool isName() const { return _name.length() > 0; }
  std::string getName() const { return _name; }
  void setName(std::string n) { _name = n; }

  bool &isWholeNumber() { return _isWholeNumber; }
  bool isWholeNumber() const { return _isWholeNumber; }
  int getWholeNumber() const { return _wholeNumber; }
  void setWholeNumber(int n)
  {
    _wholeNumber = n;
    isWholeNumber() = true;
  }

  void setString(std::string s) { _string = s; }
  std::string getString() const { return _string; }
  bool isString() const { return _string.length() > 0; }

  bool isComma() const { return _symbol == ','; }

  void setIndent(int i)
  {
    _isIndent = true;
    _indent = i;
  }
  int getIndent() const { return _indent; }
  bool isIndent() const { return _isIndent; }

  void setDedent(int d)
  {
    _isDedent = true;
    _dedent = d;
  }
  int getDedent() const { return _dedent; }
  bool isDedent() const { return _isDedent; }

  std::string print() const;

private:
  std::string _name;
  bool _eof, _eol;
  bool _isWholeNumber, _isIndent, _isDedent;
  char _symbol;
  std::string _relational;
  std::string _string;
  int _wholeNumber, _indent, _dedent;
};

#endif //EXPRINTER_TOKEN_HPP
