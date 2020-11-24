CC=c99
CFLAGS=-g -Wall -Werror -Iinclude/
LDFLAGS=-g -Iinclude/

all:
	dirs library test

library: dirs

test: library
	$(CC) $(CFLAGS) -Itest/ -c -o test/obj/test_scuttle.o test/test_scuttle.c
	$(CC) $(LDFLAGS) -Itest/ -o test/bin/test_scuttle test/obj/test_scuttle.o
	test/bin/test_scuttle

clean:
	rm -rf obj lib test/obj test/bin

install: library

dirs:
	mkdir -p obj lib test/obj test/bin
