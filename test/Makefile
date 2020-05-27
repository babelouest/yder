#
# Public domain, no copyright. Use at your own risk.
#

CC=gcc
CFLAGS+=-Wall -D_REENTRANT -DDEBUG -g -O0 -I$(INCLUDE_LOCATION) $(CPPFLAGS)
YDER_LOCATION=../src
INCLUDE_LOCATION=../include
LIBS=-lc -lorcania -lyder $(shell pkg-config --libs check) -L$(YDER_LOCATION)

all: test

clean:
	rm -f *.o yder_test valgrind.txt

libyder.so:
	cd $(YDER_LOCATION) && $(MAKE) debug

yder_test: yder_test.c
	$(CC) $(CFLAGS) yder_test.c -o yder_test $(LIBS)

test_yder_test: libyder.so yder_test
	LD_LIBRARY_PATH=$(YDER_LOCATION):${LD_LIBRARY_PATH} ./yder_test

test: test_yder_test

memcheck: yder_test
	LD_LIBRARY_PATH=$(YDER_LOCATION):${LD_LIBRARY_PATH} valgrind --tool=memcheck --leak-check=full --show-leak-kinds=all ./yder_test 2>valgrind.txt
