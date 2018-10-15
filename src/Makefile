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
YDER_INCLUDE=../include
CONFIG_TEMPLATE=$(YDER_INCLUDE)/yder-cfg.h.in
CONFIG_FILE=$(YDER_INCLUDE)/yder-cfg.h
CC=gcc
CFLAGS+=-c -fPIC -Wall -D_REENTRANT -I$(YDER_INCLUDE) $(ADDITIONALFLAGS)
LIBS=-lc -lorcania $(ADDITIONALLIBS)
OUTPUT=libyder.so
VERSION=1.4.3

ifndef Y_DISABLE_JOURNALD
	DISABLE_JOURNALD=0
	LIBS+=-lsystemd
else
	DISABLE_JOURNALD=1
endif

all: release

config:
	@if [ -f $(CONFIG_FILE) ]; then rm -f $(CONFIG_FILE); fi
	@cp $(CONFIG_TEMPLATE) $(CONFIG_FILE)
	@sed -i -e 's/$${PROJECT_VERSION}/$(VERSION)/g' $(CONFIG_FILE)        
	@if [ "$(DISABLE_JOURNALD)" = "1" ]; then \
		sed -i -e 's/\#cmakedefine DISABLE_JOURNALD/\#define Y_DISABLE_JOURNALD/g' $(CONFIG_FILE); \
	else \
		sed -i -e 's/\#cmakedefine DISABLE_JOURNALD/\/* #undef Y_DISABLE_JOURNALD *\//g' $(CONFIG_FILE); \
	fi
	@echo Config file $(CONFIG_FILE) generated 

libyder.so: config yder.o
	$(CC) -shared -fPIC -Wl,-soname,$(OUTPUT) -o $(OUTPUT).$(VERSION) yder.o $(LIBS) $(LDFLAGS)
	ln -sf $(OUTPUT).$(VERSION) $(OUTPUT)

libyder.a: config yder.o
	ar rcs libyder.a yder.o

yder.o: $(YDER_INCLUDE)/yder.h yder.c
	$(CC) $(CFLAGS) $(CPPFLAGS) yder.c

clean:
	rm -f *.o *.so *.a $(OUTPUT) $(OUTPUT).* $(YDER_INCLUDE)/yder-cfg.h

install: all
	install $(OUTPUT).$(VERSION) $(DESTDIR)/lib
	install $(YDER_INCLUDE)/yder.h $(DESTDIR)/include
	install $(YDER_INCLUDE)/yder-cfg.h $(DESTDIR)/include
	-ldconfig

static-install: static
	install libyder.a $(DESTDIR)/lib
	install $(YDER_INCLUDE)/yder.h $(DESTDIR)/include
	install $(YDER_INCLUDE)/yder-cfg.h $(DESTDIR)/include

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

