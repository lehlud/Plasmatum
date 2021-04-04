FILES = src/main.cc src/utils.cc src/lex.yy.cc src/parser.tab.cc

bin: flex-bison
	mkdir -p bin
	g++ $(FILES) -O3 -o bin/plsm


flex-bison:
	flex --outfile=src/lex.yy.cc src/lexer.ll
	bison src/parser.yy
	mv parser.tab.cc src/
	mv parser.tab.hh src/
