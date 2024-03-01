#
# Public domain, no copyright. Use at your own risk.
#

CC=gcc
CFLAGS+=-Wall -Werror -Wextra -D_REENTRANT -DDEBUG -g -O0 -I$(INCLUDE_LOCATION) $(CPPFLAGS)
YDER_LOCATION=../src
INCLUDE_LOCATION=../include
YDER_LIBRARY=$(YDER_LOCATION)/libyder.so
LDFLAGS=-lc -lorcania -lyder $(shell pkg-config --libs check) -L$(YDER_LOCATION)
TARGET=yder_test
VALGRIND_COMMAND=valgrind --tool=memcheck --leak-check=full --show-leak-kinds=all
VERBOSE=0
MEMCHECK=0

all: test

clean:
	rm -f *.o $(TARGET) *.log valgrind-*.txt

$(YDER_LIBRARY): $(YDER_LOCATION)/yder.c $(INCLUDE_LOCATION)/yder.h
	cd $(YDER_LOCATION) && $(MAKE) debug  $*

%: $(YDER_LIBRARY) %.c
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS)

test: $(YDER_LIBRARY) $(TARGET) test_yder_test

check: test

test_%: % $(YDER_LIBRARY)
	@if [ "$(VERBOSE)" = "0" ] && [ "$(MEMCHECK)" = "0" ]; then \
		LD_LIBRARY_PATH=$(YDER_LOCATION):${LD_LIBRARY_PATH} ./run_test.sh ./$^; \
	elif [ "$(MEMCHECK)" = "0" ]; then \
		LD_LIBRARY_PATH=$(YDER_LOCATION):${LD_LIBRARY_PATH} ./$^ ; \
	else \
		LD_LIBRARY_PATH=$(YDER_LOCATION):${LD_LIBRARY_PATH} $(VALGRIND_COMMAND) ./$^ 2>valgrind-$@.txt; \
	fi
