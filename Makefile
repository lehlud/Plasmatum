STD_SRC_FILES = $(wildcard src/*.cc src/AST/*.cc)
LEX_BISON_SRC_FILES += src/parser.tab.cc src/lex.yy.cc
SRC_FILES = $(filter-out $(LEX_BISON_SRC_FILES), $(STD_SRC_FILES)) $(LEX_BISON_SRC_FILES)
OBJECT_FILES = $(SRC_FILES:.cc=.o)

C_FLAGS = -O3
CXX_FLAGS = $(shell llvm-config --cxxflags)
LLVM_LD_FLAGS = $(shell llvm-config --ldflags --libs all) -flto

LIB_SRC_FILES = $(wildcard src/lib/*.c)
LIB_OBJECT_FILES = $(LIB_SRC_FILES:.c=.o)

$(shell mkdir -p output)

all: output/plasmatum output/libplsm.so

output/libplsm.so: $(LIB_OBJECT_FILES)
	clang $(LIB_OBJECT_FILES) -shared -o output/libplsm.so -flto -lm
	strip output/libplsm.so

output/plasmatum: $(OBJECT_FILES)
	clang++ $(OBJECT_FILES) -o output/plasmatum $(LLVM_LD_FLAGS)
	strip output/plasmatum

%.o: %.c
	clang -c -o $@ $< $(C_FLAGS)

%.o: %.cc
	clang++ -c $< -o $@ $(CXX_FLAGS)

src/lex.yy.cc: src/lexer.ll src/parser.tab.hh
	flex --outfile=src/lex.yy.cc src/lexer.ll

src/parser.tab.hh src/parser.tab.cc: src/parser.yy
	bison src/parser.yy
	mv parser.tab.cc src/
	mv parser.tab.hh src/

clean:
	rm -f src/parser.tab.hh
	rm -f $(LEX_BISON_SRC_FILES)
	rm -f $(OBJECT_FILES)
	rm -rf output/
	mkdir -p output
