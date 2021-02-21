#!/bin/sh

cd src
flex lexer.l
bison -y parser.y
bison -y -d parser.y

gcc -lfl lex.yy.c y.tab.c -lm -o ../a.out