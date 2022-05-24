#
# Example program
#
# Makefile used to build the software
#
# Copyright 2014-2020 Nicolas Mora <mail@babelouest.org>
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
CFLAGS=-c -Wall -D_REENTRANT $(ADDITIONALFLAGS) -I$(INCLUDE_LOCATION)
YDER_LOCATION=../src
INCLUDE_LOCATION=../include
LIBS=-lc -lyder -L$(YDER_LOCATION)
ADDITIONALFLAGS=-O3
TARGET=log_console log_syslog log_file log_combined log_callback log_newline

ifndef Y_DISABLE_JOURNALD
	TARGET+= log_journald
endif

all: libyder.so $(TARGET)

clean:
	rm -f *.o valgrind.txt log_console log_syslog log_file log_combined log_journald log_callback log_newline

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

log_callback: log_callback.c
	$(CC) -c $(CFLAGS) log_callback.c
	$(CC) -o log_callback log_callback.o $(LIBS)

log_newline: log_newline.c
	$(CC) -c $(CFLAGS) log_newline.c
	$(CC) -o log_newline log_newline.o $(LIBS)

test_console: libyder.so log_console
	LD_LIBRARY_PATH=$(YDER_LOCATION):${LD_LIBRARY_PATH} ./log_console

test_syslog: libyder.so log_syslog
	LD_LIBRARY_PATH=$(YDER_LOCATION):${LD_LIBRARY_PATH} ./log_syslog

test_file: libyder.so log_file
	LD_LIBRARY_PATH=$(YDER_LOCATION):${LD_LIBRARY_PATH} ./log_file

test_combined: libyder.so log_combined
	LD_LIBRARY_PATH=$(YDER_LOCATION):${LD_LIBRARY_PATH} ./log_combined

test_journald: libyder.so log_journald
	LD_LIBRARY_PATH=$(YDER_LOCATION):${LD_LIBRARY_PATH} ./log_journald

test_callback: libyder.so log_callback
	LD_LIBRARY_PATH=$(YDER_LOCATION):${LD_LIBRARY_PATH} ./log_callback

test_newline: libyder.so log_newline
	LD_LIBRARY_PATH=$(YDER_LOCATION):${LD_LIBRARY_PATH} ./log_newline

ifndef Y_DISABLE_JOURNALD
test: libyder.so $(TARGET)
	LD_LIBRARY_PATH=$(YDER_LOCATION):${LD_LIBRARY_PATH} ./log_console
	LD_LIBRARY_PATH=$(YDER_LOCATION):${LD_LIBRARY_PATH} ./log_syslog
	LD_LIBRARY_PATH=$(YDER_LOCATION):${LD_LIBRARY_PATH} ./log_file
	LD_LIBRARY_PATH=$(YDER_LOCATION):${LD_LIBRARY_PATH} ./log_combined
	LD_LIBRARY_PATH=$(YDER_LOCATION):${LD_LIBRARY_PATH} ./log_journald
	LD_LIBRARY_PATH=$(YDER_LOCATION):${LD_LIBRARY_PATH} ./log_callback
	LD_LIBRARY_PATH=$(YDER_LOCATION):${LD_LIBRARY_PATH} ./log_newline
else
	LD_LIBRARY_PATH=$(YDER_LOCATION):${LD_LIBRARY_PATH} ./log_console
	LD_LIBRARY_PATH=$(YDER_LOCATION):${LD_LIBRARY_PATH} ./log_syslog
	LD_LIBRARY_PATH=$(YDER_LOCATION):${LD_LIBRARY_PATH} ./log_file
	LD_LIBRARY_PATH=$(YDER_LOCATION):${LD_LIBRARY_PATH} ./log_combined
	LD_LIBRARY_PATH=$(YDER_LOCATION):${LD_LIBRARY_PATH} ./log_callback
	LD_LIBRARY_PATH=$(YDER_LOCATION):${LD_LIBRARY_PATH} ./log_newline
endif

ifndef Y_DISABLE_JOURNALD
memcheck: libyder.so $(TARGET)
	LD_LIBRARY_PATH=$(YDER_LOCATION):${LD_LIBRARY_PATH} valgrind --tool=memcheck --leak-check=full --show-leak-kinds=all ./log_console 2>valgrind.txt
	LD_LIBRARY_PATH=$(YDER_LOCATION):${LD_LIBRARY_PATH} valgrind --tool=memcheck --leak-check=full --show-leak-kinds=all ./log_syslog 2>>valgrind.txt
	LD_LIBRARY_PATH=$(YDER_LOCATION):${LD_LIBRARY_PATH} valgrind --tool=memcheck --leak-check=full --show-leak-kinds=all ./log_file 2>>valgrind.txt
	LD_LIBRARY_PATH=$(YDER_LOCATION):${LD_LIBRARY_PATH} valgrind --tool=memcheck --leak-check=full --show-leak-kinds=all ./log_combined 2>>valgrind.txt
	LD_LIBRARY_PATH=$(YDER_LOCATION):${LD_LIBRARY_PATH} valgrind --tool=memcheck --leak-check=full --show-leak-kinds=all ./log_journald 2>>valgrind.txt
	LD_LIBRARY_PATH=$(YDER_LOCATION):${LD_LIBRARY_PATH} valgrind --tool=memcheck --leak-check=full --show-leak-kinds=all ./log_callback 2>>valgrind.txt
	LD_LIBRARY_PATH=$(YDER_LOCATION):${LD_LIBRARY_PATH} valgrind --tool=memcheck --leak-check=full --show-leak-kinds=all ./log_newline 2>>valgrind.txt
else
	LD_LIBRARY_PATH=$(YDER_LOCATION):${LD_LIBRARY_PATH} valgrind --tool=memcheck --leak-check=full --show-leak-kinds=all ./log_console 2>valgrind.txt
	LD_LIBRARY_PATH=$(YDER_LOCATION):${LD_LIBRARY_PATH} valgrind --tool=memcheck --leak-check=full --show-leak-kinds=all ./log_syslog 2>>valgrind.txt
	LD_LIBRARY_PATH=$(YDER_LOCATION):${LD_LIBRARY_PATH} valgrind --tool=memcheck --leak-check=full --show-leak-kinds=all ./log_file 2>>valgrind.txt
	LD_LIBRARY_PATH=$(YDER_LOCATION):${LD_LIBRARY_PATH} valgrind --tool=memcheck --leak-check=full --show-leak-kinds=all ./log_combined 2>>valgrind.txt
	LD_LIBRARY_PATH=$(YDER_LOCATION):${LD_LIBRARY_PATH} valgrind --tool=memcheck --leak-check=full --show-leak-kinds=all ./log_callback 2>>valgrind.txt
	LD_LIBRARY_PATH=$(YDER_LOCATION):${LD_LIBRARY_PATH} valgrind --tool=memcheck --leak-check=full --show-leak-kinds=all ./log_newline 2>>valgrind.txt
endif
