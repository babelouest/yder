#
# Yder Framework
#
# Makefile used to build the software
#
# Copyright 2015-2018 Nicolas Mora <mail@babelouest.org>
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
DESTDIR=/usr/local
CC=gcc
CFLAGS+=-c -fPIC -Wall -D_REENTRANT $(ADDITIONALFLAGS)
LIBS=-lc -lorcania $(ADDITIONALLIBS)
OUTPUT=libyder.so
VERSION=1.4.2

ifndef Y_DISABLE_JOURNALD
	LIBS+=-lsystemd
endif

all: release

libyder.so: yder.o
	$(CC) -shared -fPIC -Wl,-soname,$(OUTPUT) -o $(OUTPUT).$(VERSION) yder.o $(LIBS) $(LDFLAGS)
	ln -sf $(OUTPUT).$(VERSION) $(OUTPUT)

libyder.a: yder.o
	ar rcs libyder.a yder.o

yder.o: ../include/yder.h yder.c
	$(CC) $(CFLAGS) $(CPPFLAGS) yder.c

clean:
	rm -f *.o *.so *.a $(OUTPUT) $(OUTPUT).*

install: all
	cp $(OUTPUT).$(VERSION) $(DESTDIR)/lib
	cp ../include/yder.h $(DESTDIR)/include
	-ldconfig

static-install: static
	cp libyder.a $(DESTDIR)/lib
	cp ../include/yder.h $(DESTDIR)/include

uninstall:
	rm -f $(DESTDIR)/lib/$(OUTPUT) $(DESTDIR)/lib/libyder.a
	rm -f $(DESTDIR)/lib/$(OUTPUT).*
	rm -f $(DESTDIR)/include/yder.h

debug: ADDITIONALFLAGS=-DDEBUG -g -O0

debug: libyder.so

release: ADDITIONALFLAGS=-O3

release: libyder.so

static-debug: ADDITIONALFLAGS=-DDEBUG -g -O0

static-debug: libyder.a

static: ADDITIONALFLAGS=-O3

static: libyder.a

