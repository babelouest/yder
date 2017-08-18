#
# Yder Framework
#
# Makefile used to build the software
#
# Copyright 2015-2016 Nicolas Mora <mail@babelouest.org>
#
# This program is free software; you can redistribute it and/or
# modify it under the terms of the GNU Lesser General Public License
# as published by the Free Software Foundation;
# version 2.1 of the License.
#
# This library is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.	See the
# GNU GENERAL PUBLIC LICENSE for more details.
#
# You should have received a copy of the GNU General Public
# License along with this library.	If not, see <http://www.gnu.org/licenses/>.
#
PREFIX=/usr/local
CC=gcc
CFLAGS=-c -fPIC -Wall -D_REENTRANT $(ADDITIONALFLAGS)
LIBS=-lc -lorcania
OUTPUT=libyder.so
VERSION=2.0

all: release

libyder.so: yder.o
	$(CC) -shared -Wl,-soname,$(OUTPUT) -o $(OUTPUT).$(VERSION) yder.o $(LIBS)
	ln -sf $(OUTPUT).$(VERSION) $(OUTPUT)

libyder.a: yder.o
	ar rcs libyder.a yder.o

yder.o: yder.h yder.c
	$(CC) $(CFLAGS) yder.c

clean:
	rm -f *.o *.so *.a $(OUTPUT) $(OUTPUT).*

install: all
	cp $(OUTPUT).$(VERSION) $(PREFIX)/lib
	cp yder.h $(PREFIX)/include
	-ldconfig

static-install: static
	cp libyder.a $(PREFIX)/lib
	cp yder.h $(PREFIX)/include

uninstall:
	rm -f $(PREFIX)/lib/$(OUTPUT) $(PREFIX)/lib/libyder.a
	rm -f $(PREFIX)/lib/$(OUTPUT).*
	rm -f $(PREFIX)/include/yder.h

debug: ADDITIONALFLAGS=-DDEBUG -g -O0

debug: libyder.so

release: ADDITIONALFLAGS=-O3

release: libyder.so

static-debug: ADDITIONALFLAGS=-DDEBUG -g -O0

static-debug: libyder.a

static: ADDITIONALFLAGS=-O3

static: libyder.a

