.SUFFIXES: .o .cpp .x

CFLAGS = -ggdb -std=c++17
objects =  Token.o Tokenizer.o Parser.o Expr.o SymTab.o Statements.o mini_python.o

mini_python: $(objects)
	g++ $(CFLAGS) -o mini_python $(objects)

.cpp.o:
	g++ $(CFLAGS) -c $< -o $@


Token.o:  Token.cpp Token.hpp
Tokenizer.o: Tokenizer.cpp Tokenizer.hpp
Expr.o: Expr.cpp Expr.hpp Token.hpp SymTab.hpp
SymTab.o: SymTab.cpp SymTab.hpp Statements.hpp
Parser.o: Parser.cpp Token.hpp Parser.hpp Tokenizer.hpp SymTab.hpp Expr.hpp Statements.hpp
Statements.o: Statements.cpp Statements.hpp Expr.hpp Token.hpp SymTab.hpp
mini_python.o: mini_python.cpp Token.hpp Tokenizer.hpp Parser.hpp SymTab.hpp Expr.hpp Statements.hpp

clean:
	rm -fr *.o *~ *.x mini_python
