#!/bin/bash

g++ -ggdb -std=c++17 -c Token.cpp -o Token.o
g++ -ggdb -std=c++17 -c Tokenizer.cpp -o Tokenizer.o
g++ -ggdb -std=c++17 -c Expr.cpp -o Expr.o
g++ -ggdb -std=c++17 -c SymTab.cpp -o SymTab.o
g++ -ggdb -std=c++17 -c Parser.cpp -o Parser.o
g++ -ggdb -std=c++17 -c Statements.cpp -o Statements.o
g++ -ggdb -std=c++17 -c mini_python.cpp -o mini_python.o

g++ -ggdb -std=c++17 -o mini_python Token.o Tokenizer.o Parser.o Expr.o SymTab.o Statements.o mini_python.o
