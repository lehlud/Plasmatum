#!/bin/sh
flex src/lexer.l
bison -y src/parser.y
bison -y -d src/parser.y

cc -lfl lex.yy.c y.tab.c