CC=gcc

all:
	dirs library test

library: dirs

test: library

clean:
	rm -rf obj lib test/obj test/bin

install: library

dirs:
	mkdir -p obj lib test/obj test/bin
