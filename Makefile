.PHONY: all clean install

all:
	test

test:
	bash src/scuttle.sh test
	$(MAKE) -C test
	cat test/log/test_scuttle.log

example:
	$(MAKE) -C example test

clean:
	rm -rf obj lib test/obj test/bin

install:
	install -g root -o root -m 644 include/scuttle.h /usr/local/include/
	install -g root -o root -m 644 -D -t /usr/local/lib/scuttle src/scuttle.sh
