STD_SRC_FILES = $(wildcard src/*.cc src/AST/*.cc)
LEX_BISON_SRC_FILES += src/parser.tab.cc src/lex.yy.cc
SRC_FILES = $(filter-out $(LEX_BISON_SRC_FILES), $(STD_SRC_FILES)) $(LEX_BISON_SRC_FILES)
OBJECT_FILES = $(SRC_FILES:.cc=.o)

all: $(OBJECT_FILES)
	mkdir -p output
	g++ $(OBJECT_FILES) -o output/plasmatum
	strip output/plasmatum

%.o: %.cc
	g++ -c $< -o $@

src/lex.yy.cc: src/lexer.ll src/parser.tab.hh
	flex --outfile=src/lex.yy.cc src/lexer.ll

src/parser.tab.hh src/parser.tab.cc: src/parser.yy
	bison src/parser.yy
	mv parser.tab.cc src/
	mv parser.tab.hh src/

clean:
	rm -f src/lex.yy.cc
	rm -f src/parser.tab.hh
	rm -f src/parser.tab.cc
	rm -f $(OBJECT_FILES)
	rm -rf output/
