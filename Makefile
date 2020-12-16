.PHONY: all clean install test example

all:
	test

test:
	bash src/scuttle.sh -o test
	$(MAKE) -C test
	cat test/log/test_scuttle.log

example:
	$(MAKE) -C example all test

clean:
	$(MAKE) -C test clean
	rm -rf obj lib

install:
	install -g root -o root -m 644 include/scuttle.h /usr/local/include/
	install -g root -o root -m 644 -D -t /usr/local/lib/scuttle src/scuttle.sh
