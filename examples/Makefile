#
# Example program
#
# Makefile used to build the software
#
# Copyright 2014-2015 Nicolas Mora <mail@babelouest.org>
#
# This program is free software; you can redistribute it and/or
# modify it under the terms of the MIT License
#
# This library is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU GENERAL PUBLIC LICENSE for more details.
#
CC=gcc
CFLAGS=-c -Wall -D_REENTRANT $(ADDITIONALFLAGS)
YDER_LOCATION=../src
LIBS=-lc -lyder -L$(YDER_LOCATION)
ADDITIONALFLAGS=-O3
TARGET=log_console log_syslog log_file log_combined

ifndef Y_DISABLE_JOURNALD
	TARGET+= log_journald
endif

all: libyder.so $(TARGET)

clean:
	rm -f *.o log_console log_syslog log_file log_combined log_journald

debug: ADDITIONALFLAGS=-DDEBUG -g -O0

debug: libyder.so $(TARGET)

libyder.so:
	cd $(YDER_LOCATION) && $(MAKE)

log_console: log_console.c
	$(CC) -c $(CFLAGS) log_console.c
	$(CC) -o log_console log_console.o $(LIBS)

log_syslog: log_syslog.c
	$(CC) -c $(CFLAGS) log_syslog.c
	$(CC) -o log_syslog log_syslog.o $(LIBS)

log_journald: log_journald.c
	$(CC) -c $(CFLAGS) log_journald.c
	$(CC) -o log_journald log_journald.o $(LIBS)

log_file: log_file.c
	$(CC) -c $(CFLAGS) log_file.c
	$(CC) -o log_file log_file.o $(LIBS)

log_combined: log_combined.c
	$(CC) -c $(CFLAGS) log_combined.c
	$(CC) -o log_combined log_combined.o $(LIBS)

test_console: libyder.so $(TARGET)
	LD_LIBRARY_PATH=$(YDER_LOCATION):${LD_LIBRARY_PATH} ./log_console

test_syslog: libyder.so $(TARGET)
	LD_LIBRARY_PATH=$(YDER_LOCATION):${LD_LIBRARY_PATH} ./log_syslog

test_file: libyder.so $(TARGET)
	LD_LIBRARY_PATH=$(YDER_LOCATION):${LD_LIBRARY_PATH} ./log_file

test_combined: libyder.so $(TARGET)
	LD_LIBRARY_PATH=$(YDER_LOCATION):${LD_LIBRARY_PATH} ./log_combined

test_journald: libyder.so $(TARGET)
	LD_LIBRARY_PATH=$(YDER_LOCATION):${LD_LIBRARY_PATH} ./log_journald

ifndef Y_DISABLE_JOURNALD
test: libyder.so $(TARGET)
	LD_LIBRARY_PATH=$(YDER_LOCATION):${LD_LIBRARY_PATH} ./log_console
	LD_LIBRARY_PATH=$(YDER_LOCATION):${LD_LIBRARY_PATH} ./log_syslog
	LD_LIBRARY_PATH=$(YDER_LOCATION):${LD_LIBRARY_PATH} ./log_file
	LD_LIBRARY_PATH=$(YDER_LOCATION):${LD_LIBRARY_PATH} ./log_combined
	LD_LIBRARY_PATH=$(YDER_LOCATION):${LD_LIBRARY_PATH} ./log_journald
else
	LD_LIBRARY_PATH=$(YDER_LOCATION):${LD_LIBRARY_PATH} ./log_console
	LD_LIBRARY_PATH=$(YDER_LOCATION):${LD_LIBRARY_PATH} ./log_syslog
	LD_LIBRARY_PATH=$(YDER_LOCATION):${LD_LIBRARY_PATH} ./log_file
	LD_LIBRARY_PATH=$(YDER_LOCATION):${LD_LIBRARY_PATH} ./log_combined
endif

ifndef Y_DISABLE_JOURNALD
memcheck: libyder.so log_console log_syslog log_file log_combined log_journald
	LD_LIBRARY_PATH=$(YDER_LOCATION):${LD_LIBRARY_PATH} valgrind --tool=memcheck --leak-check=full --show-leak-kinds=all ./log_console 2>valgrind.txt
	LD_LIBRARY_PATH=$(YDER_LOCATION):${LD_LIBRARY_PATH} valgrind --tool=memcheck --leak-check=full --show-leak-kinds=all ./log_syslog 2>>valgrind.txt
	LD_LIBRARY_PATH=$(YDER_LOCATION):${LD_LIBRARY_PATH} valgrind --tool=memcheck --leak-check=full --show-leak-kinds=all ./log_file 2>>valgrind.txt
	LD_LIBRARY_PATH=$(YDER_LOCATION):${LD_LIBRARY_PATH} valgrind --tool=memcheck --leak-check=full --show-leak-kinds=all ./log_combined 2>>valgrind.txt
	LD_LIBRARY_PATH=$(YDER_LOCATION):${LD_LIBRARY_PATH} valgrind --tool=memcheck --leak-check=full --show-leak-kinds=all ./log_journald 2>>valgrind.txt
else
	LD_LIBRARY_PATH=$(YDER_LOCATION):${LD_LIBRARY_PATH} valgrind --tool=memcheck --leak-check=full --show-leak-kinds=all ./log_console 2>valgrind.txt
	LD_LIBRARY_PATH=$(YDER_LOCATION):${LD_LIBRARY_PATH} valgrind --tool=memcheck --leak-check=full --show-leak-kinds=all ./log_syslog 2>>valgrind.txt
	LD_LIBRARY_PATH=$(YDER_LOCATION):${LD_LIBRARY_PATH} valgrind --tool=memcheck --leak-check=full --show-leak-kinds=all ./log_file 2>>valgrind.txt
	LD_LIBRARY_PATH=$(YDER_LOCATION):${LD_LIBRARY_PATH} valgrind --tool=memcheck --leak-check=full --show-leak-kinds=all ./log_combined 2>>valgrind.txt
endif
