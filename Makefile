CFLAGS = -g -O0

test: test.c alloc.c

clean:
	-rm -rf *.o test test.dSYM/
