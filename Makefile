CC = clang
CFLAGS = -Weverything -g -O0 -std=c17 -Wno-cast-align

test: test.c alloc.c

clean:
	-rm -rf *.o test test.dSYM/
