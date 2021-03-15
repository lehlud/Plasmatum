#!/bin/sh

cd src
flex --outfile=lex.yy.cc lexer.ll
bison parser.yy

clang++ -Wextra -Wall -lfl main.cc utils.cc lex.yy.cc parser.tab.cc -lm -O3 -s -o ../plsm
