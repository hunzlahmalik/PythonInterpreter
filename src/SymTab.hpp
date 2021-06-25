#ifndef EXPRINTER_SYMTAB_HPP
#define EXPRINTER_SYMTAB_HPP

#include <string>
#include <map>
#include <vector>
class Functions;

// This is a flat and integer-based symbol table. It allows for variables to be
// initialized, determines if a give variable has been defined or not, and if
// a variable has been defined, it returns its value.

//head to types
struct TypeDescriptor
{

  enum types
  {
    INTEGER,
    DOUBLE,
    BOOL,
    STRING,
    INTARRAY,
    STRARRAY
  };

  TypeDescriptor(types type) : _type{type} {}

  types &type() { return _type; }

  virtual ~TypeDescriptor() {}

protected:
  types _type;
};

//number type
struct NumberDescriptor : TypeDescriptor
{
  NumberDescriptor(types descType) : TypeDescriptor(descType) {}
  union
  {
    int intValue;
    double doubleValue;
    short boolValue;
  } value;
};

//string type
struct StringDescriptor : TypeDescriptor
{
  StringDescriptor(types descType) : TypeDescriptor(descType) {}
  std::string value;
};

//arrya type
struct ArrayDescriptor : TypeDescriptor
{
  ArrayDescriptor(types descType) : TypeDescriptor(descType) {}
  std::vector<int> valueInt;
  std::vector<std::string> valueString;
};

//symbol table
class SymTab
{
public:
  SymTab();
  ~SymTab();
  void setValueFor(std::string vName, TypeDescriptor *value);
  bool isDefined(std::string vName);
  bool isDefinedOwnScope(std::string vName);
  TypeDescriptor *getValueFor(std::string vName);
  std::string getValueString(std::string vName);
  void openScope();
  void storeReturnValue(TypeDescriptor *val);
  TypeDescriptor *getReturnValue();
  void closeScope();
  Functions *getFunctions() { return _func; };

private:
  std::vector<std::map<std::string, TypeDescriptor *>> symTab;
  Functions *_func;
  TypeDescriptor *_return;
};

#endif //EXPRINTER_SYMTAB_HPP
