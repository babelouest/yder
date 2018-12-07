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
CFLAGS+=-c -fPIC -Wall -D_REENTRANT -I$(YDER_INCLUDE) $(ADDITIONALFLAGS) $(CPPFLAGS)
LIBS=-lc -lorcania $(ADDITIONALLIBS)
OUTPUT=libyder.so
VERSION=1.4.4

ifndef Y_DISABLE_JOURNALD
	DISABLE_JOURNALD=0
	LIBS+=-lsystemd
else
	DISABLE_JOURNALD=1
endif

all: release

$(CONFIG_FILE):
	@cp $(CONFIG_TEMPLATE) $(CONFIG_FILE)
	@echo Config file $(CONFIG_FILE) generated 
	@sed -i -e 's/$${PROJECT_VERSION}/$(VERSION)/g' $(CONFIG_FILE)        
	@if [ "$(DISABLE_JOURNALD)" = "1" ]; then \
		echo "JOURNALD SUPPORT DISABLED"; \
		sed -i -e 's/\#cmakedefine DISABLE_JOURNALD/\#define Y_DISABLE_JOURNALD/g' $(CONFIG_FILE); \
	else \
		echo "JOURNALD SUPPORT ENABLED"; \
		sed -i -e 's/\#cmakedefine DISABLE_JOURNALD/\/* #undef Y_DISABLE_JOURNALD *\//g' $(CONFIG_FILE); \
	fi

libyder.so: $(CONFIG_FILE) yder.o
	$(CC) -shared -fPIC -Wl,-soname,$(OUTPUT) -o $(OUTPUT).$(VERSION) yder.o $(LIBS) $(LDFLAGS)
	ln -sf $(OUTPUT).$(VERSION) $(OUTPUT)

libyder.a: $(CONFIG_FILE) yder.o
	ar rcs libyder.a yder.o

yder.o: $(YDER_INCLUDE)/yder.h yder.c
	$(CC) $(CFLAGS) $(CPPFLAGS) yder.c

clean:
	rm -f *.o *.so *.a $(OUTPUT) $(OUTPUT).* $(YDER_INCLUDE)/yder-cfg.h

install: all
	install $(OUTPUT).$(VERSION) $(DESTDIR)/lib
	install -m644 $(YDER_INCLUDE)/yder.h $(DESTDIR)/include
	install -m644 $(CONFIG_FILE) $(DESTDIR)/include
	-ldconfig

static-install: static
	install libyder.a $(DESTDIR)/lib
	install -m644 $(YDER_INCLUDE)/yder.h $(DESTDIR)/include
	install -m644 $(CONFIG_FILE) $(DESTDIR)/include

uninstall:
	rm -f $(DESTDIR)/lib/$(OUTPUT) $(DESTDIR)/lib/libyder.a
	rm -f $(DESTDIR)/lib/$(OUTPUT).*
	rm -f $(DESTDIR)/include/yder.h
	rm -f $(DESTDIR)/include/yder-cfg.h

debug: ADDITIONALFLAGS=-DDEBUG -g -O0

debug: libyder.so

release: ADDITIONALFLAGS=-O3

release: libyder.so

static-debug: ADDITIONALFLAGS=-DDEBUG -g -O0

static-debug: libyder.a

static: ADDITIONALFLAGS=-O3

static: libyder.a

