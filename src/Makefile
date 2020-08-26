#
# Yder Framework
#
# Makefile used to build the software
#
# Copyright 2015-2020 Nicolas Mora <mail@babelouest.org>
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
PROJECT_NAME=yder
PROJECT_DESCRIPTION=Logging library for C applications
PROJECT_BUGREPORT_PATH=https://github.com/babelouest/yder/issues
PKGCONF_REQ=
PKGCONF_REQ_PRIVATE=
DESTDIR=/usr/local
YDER_INCLUDE=../include
CONFIG_TEMPLATE=$(YDER_INCLUDE)/yder-cfg.h.in
CONFIG_FILE=$(YDER_INCLUDE)/yder-cfg.h
PKGCONFIG_FILE=../libyder.pc
PKGCONFIG_TEMPLATE=../libyder.pc.in
CC=gcc
CFLAGS+=-c -fPIC -Wall -Werror -Wextra -D_REENTRANT -I$(YDER_INCLUDE) $(ADDITIONALFLAGS) $(CPPFLAGS)
LIBS=-lc -lorcania $(ADDITIONALLIBS)
OUTPUT=libyder.so
VERSION_MAJOR=1
VERSION_MINOR=4
VERSION_PATCH=12

SONAME = -soname
ifeq ($(shell uname -s),Darwin)
	SONAME = -install_name
endif

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
	@sed -i -e 's/$${PROJECT_VERSION}/$(VERSION_MAJOR).$(VERSION_MINOR).$(VERSION_PATCH)/g' $(CONFIG_FILE)        
	@sed -i -e 's/$${PROJECT_VERSION_MAJOR}/$(VERSION_MAJOR)/g' $(CONFIG_FILE)        
	@sed -i -e 's/$${PROJECT_VERSION_MINOR}/$(VERSION_MINOR)/g' $(CONFIG_FILE)        
	@sed -i -e 's/$${PROJECT_VERSION_PATCH}/$(VERSION_PATCH)/g' $(CONFIG_FILE)        
	@sed -i -e 's/$${PROJECT_VERSION_NUMBER}/$(shell printf '%02d' $(VERSION_MAJOR))$(shell printf '%02d' $(VERSION_MINOR))$(shell printf '%02d' $(VERSION_PATCH))/g' $(CONFIG_FILE)        
	@if [ "$(DISABLE_JOURNALD)" = "1" ]; then \
		echo "JOURNALD SUPPORT DISABLED"; \
		sed -i -e 's/\#cmakedefine Y_DISABLE_JOURNALD/\#define Y_DISABLE_JOURNALD/g' $(CONFIG_FILE); \
	else \
		echo "JOURNALD SUPPORT ENABLED"; \
		sed -i -e 's/\#cmakedefine Y_DISABLE_JOURNALD/\/* #undef Y_DISABLE_JOURNALD *\//g' $(CONFIG_FILE); \
	fi

$(PKGCONFIG_FILE):
	@cp $(PKGCONFIG_TEMPLATE) $(PKGCONFIG_FILE)
	@echo Pkgconfig file $(PKGCONFIG_FILE) generated
	@sed -i -e 's#@CMAKE_INSTALL_PREFIX@#$(DESTDIR)#g' $(PKGCONFIG_FILE)
	@sed -i -e 's/@CMAKE_INSTALL_LIBDIR@/lib/g' $(PKGCONFIG_FILE)
	@sed -i -e 's/@CMAKE_INSTALL_INCLUDEDIR@/include/g' $(PKGCONFIG_FILE)
	@sed -i -e 's/@PROJECT_NAME@/$(PROJECT_NAME)/g' $(PKGCONFIG_FILE)
	@sed -i -e 's/@PROJECT_DESCRIPTION@/$(PROJECT_DESCRIPTION)/g' $(PKGCONFIG_FILE)
	@sed -i -e 's|@PROJECT_BUGREPORT_PATH@|$(PROJECT_BUGREPORT_PATH)|g' $(PKGCONFIG_FILE)
	@sed -i -e 's/@LIBRARY_VERSION@/$(VERSION_MAJOR).$(VERSION_MINOR).$(VERSION_PATCH)/g' $(PKGCONFIG_FILE)
	@sed -i -e 's/@PKGCONF_REQ@/$(PKGCONF_REQ)/g' $(PKGCONFIG_FILE)
	@sed -i -e 's/@PKGCONF_REQ_PRIVATE@/$(PKGCONF_REQ_PRIVATE)/g' $(PKGCONFIG_FILE)

libyder.so: yder.o
	$(CC) -shared -fPIC -Wl,$(SONAME),$(OUTPUT) -o $(OUTPUT).$(VERSION_MAJOR).$(VERSION_MINOR).$(VERSION_PATCH) yder.o $(LIBS) $(LDFLAGS)
	ln -sf $(OUTPUT).$(VERSION_MAJOR).$(VERSION_MINOR).$(VERSION_PATCH) $(OUTPUT)

libyder.a: yder.o
	ar rcs libyder.a yder.o

yder.o: $(CONFIG_FILE) $(YDER_INCLUDE)/yder.h yder.c
	$(CC) $(CFLAGS) yder.c

clean:
	rm -f *.o *.so *.a $(OUTPUT) $(PKGCONFIG_FILE) $(OUTPUT).* $(YDER_INCLUDE)/yder-cfg.h

install: all $(PKGCONFIG_FILE)
	install $(OUTPUT).$(VERSION_MAJOR).$(VERSION_MINOR).$(VERSION_PATCH) $(DESTDIR)/lib
	mkdir -p $(DESTDIR)/lib/pkgconfig/
	install -m644 $(PKGCONFIG_FILE) $(DESTDIR)/lib/pkgconfig/
	install -m644 $(YDER_INCLUDE)/yder.h $(DESTDIR)/include
	install -m644 $(CONFIG_FILE) $(DESTDIR)/include
	-ldconfig $(DESTDIR)/lib

static-install: static
	install libyder.a $(DESTDIR)/lib
	install -m644 $(PKGCONFIG_FILE) $(DESTDIR)/lib/pkgconfig
	install -m644 $(YDER_INCLUDE)/yder.h $(DESTDIR)/include
	install -m644 $(CONFIG_FILE) $(DESTDIR)/include

uninstall:
	rm -f $(DESTDIR)/lib/$(OUTPUT) $(DESTDIR)/lib/libyder.a
	rm -f $(DESTDIR)/lib/$(OUTPUT).*
	rm -f $(DESTDIR)/include/yder.h
	rm -f $(DESTDIR)/include/$(CONFIG_FILE)
	rm -f $(DESTDIR)/lib/pkgconfig/$(PKGCONFIG_FILE)

debug: ADDITIONALFLAGS=-DDEBUG -g -O0

debug: libyder.so

release: ADDITIONALFLAGS=-O3

release: libyder.so

static-debug: ADDITIONALFLAGS=-DDEBUG -g -O0

static-debug: libyder.a

static: ADDITIONALFLAGS=-O3

static: libyder.a

