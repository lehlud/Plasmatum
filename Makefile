FILES = $(wildcard src/*.c)
OBJ_FILES = $(FILES:.c=.o)
COMPILER = clang

CXXFLAGS = -O3
LDFLAGS = -flto -O3

all: bin/plsm

bin/plsm: $(OBJ_FILES)
	mkdir -p bin
	$(COMPILER) $(OBJ_FILES) $(LDFLAGS) -o bin/plsm
	strip bin/plsm

%.o: %.c
	$(COMPILER) $(CXXFLAGS) -o $@ -c $<


clean:
	rm -rf bin $(OBJ_FILES)

rebuild: clean bin/plsm
