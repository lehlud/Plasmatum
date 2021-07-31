FILES = $(wildcard src/*.cc)
OBJ_FILES = $(FILES:.cc=.o)
COMPILER = clang++

CXXFLAGS = -O3 -Wall -Wextra
LDFLAGS = -flto

all: bin/plsm

bin/plsm: $(OBJ_FILES)
	mkdir -p bin
	$(COMPILER) $(OBJ_FILES) $(LDFLAGS) -o bin/plsm
	strip bin/plsm

%.o: %.cc
	$(COMPILER) $(CXXFLAGS) -o $@ -c $<


clean:
	rm -rf bin $(OBJ_FILES)

rebuild: clean bin/plsm
