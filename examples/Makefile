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

all: libyder.so log_console log_syslog log_file

clean:
	rm -f *.o log_console log_syslog log_file log_combined

debug: ADDITIONALFLAGS=-DDEBUG -g -O0

debug: libyder.so log_console log_syslog log_file log_combined

libyder.so:
	cd $(YDER_LOCATION) && $(MAKE)

log_console: log_console.c
	$(CC) -c $(CFLAGS) log_console.c
	$(CC) -o log_console log_console.o $(LIBS)

log_syslog: log_syslog.c
	$(CC) -c $(CFLAGS) log_syslog.c
	$(CC) -o log_syslog log_syslog.o $(LIBS)

log_file: log_file.c
	$(CC) -c $(CFLAGS) log_file.c
	$(CC) -o log_file log_file.o $(LIBS)

log_combined: log_combined.c
	$(CC) -c $(CFLAGS) log_combined.c
	$(CC) -o log_combined log_combined.o $(LIBS)

test: libyder.so log_console log_syslog log_file log_combined
	LD_LIBRARY_PATH=$(YDER_LOCATION):${LD_LIBRARY_PATH} ./log_console
	LD_LIBRARY_PATH=$(YDER_LOCATION):${LD_LIBRARY_PATH} ./log_syslog
	LD_LIBRARY_PATH=$(YDER_LOCATION):${LD_LIBRARY_PATH} ./log_file
	LD_LIBRARY_PATH=$(YDER_LOCATION):${LD_LIBRARY_PATH} ./log_combined
