.PHONY: dirs test all clean

all: dirs bin/example

dirs:
	mkdir -p obj bin test/obj test/bin test/log

test: dirs
	bash ../src/scuttle.sh test
	$(MAKE) -C test

obj/%.o: src/%.c include/%.h dirs
	gcc -g -Wall -Werror -Iinclude/ -o $@ -c $<

bin/example: dirs obj/foo.o obj/bar.o src/example.c include/foo.h include/bar.h include/example.h
	gcc -g -Wall -Werror -Iinclude/ -o obj/example.o -c src/example.c
	gcc -o bin/example obj/foo.o obj/bar.o obj/example.o

clean:
ifneq (,$(wildcard test/Makefile))
	$(MAKE) -C test clean
endif
	rm -rf obj bin
