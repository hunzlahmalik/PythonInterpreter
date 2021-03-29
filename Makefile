.SUFFIXES: .o .cpp .x

CFLAGS = -ggdb -std=c++17
objects =  ./src/Token.o ./src/Tokenizer.o ./src/Parser.o ./src/Expr.o ./src/SymTab.o ./src/Statements.o ./src/mini_python.o

mini_python: $(objects)
	g++ $(CFLAGS) -o mini_python $(objects)

.cpp.o:
	g++ $(CFLAGS) -c $< -o $@


Token.o:  ./src/Token.cpp ./src/Token.hpp
Tokenizer.o: ./src/Tokenizer.cpp ./src/Tokenizer.hpp
Expr.o: ./src/Expr.cpp ./src/Expr.hpp ./src/Token.hpp ./src/SymTab.hpp
SymTab.o: ./src/SymTab.cpp ./src/SymTab.hpp ./src/Statements.hpp
Parser.o: ./src/Parser.cpp ./src/Token.hpp ./src/Parser.hpp ./src/Tokenizer.hpp ./src/SymTab.hpp ./src/Expr.hpp ./src/Statements.hpp
Statements.o: ./src/Statements.cpp ./src/Statements.hpp ./src/Expr.hpp ./src/Token.hpp ./src/SymTab.hpp
mini_python.o: ./src/mini_python.cpp ./src/Token.hpp ./src/Tokenizer.hpp ./src/Parser.hpp ./src/SymTab.hpp ./src/Expr.hpp ./src/Statements.hpp

clean:
	rm -fr ./src/*.o ./src/*~ ./src/*.x mini_python
