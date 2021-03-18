FLEX_OUTPUT = lex.yy.cc
FLEX_CMD = flex --outfile=lex.yy.cc lexer.ll
BISON_OUTPUT = parser.tab.cc
BISON_CMD = bison parser.yy

COMPILER = clang++
FILES = main.cc utils.cc lex.yy.cc parser.tab.cc
FLAGS = -Wextra -Wall -lfl -lm -O3 -s
OUTFILE = ../plsm

build:
	$(FLEX_CMD)
	$(BISON_CMD)
	$(COMPILER) $(FILES) $(FLAGS) -o $(OUTFILE)
