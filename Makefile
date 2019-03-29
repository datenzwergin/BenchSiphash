CC=gcc
CFLAGS=-Wall --std=c99
SRC=ref/siphash.c ref/halfsiphash.c bench.c
BIN=bench debug

all:                    $(BIN)

bench:                  $(SRC)
			$(CC) $(CFLAGS) $^ -o $@

debug:                  $(SRC)
			$(CC) $(CFLAGS) $^ -o $@ -DDEBUG

clean:
			rm -rf *.o $(BIN)
format:
		        clang-format -style="{BasedOnStyle: llvm, IndentWidth: 4}" \
			-i bench.c
