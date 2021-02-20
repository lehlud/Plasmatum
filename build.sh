#!/bin/sh
flex src/lexer.l
bison -y src/parser.y
bison -y -d src/parser.y

gcc -lfl lex.yy.c y.tab.c