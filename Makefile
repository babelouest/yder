#
# Example program
#
# Makefile used to build all programs
#
# Copyright 2014-2018 Nicolas Mora <mail@babelouest.org>
#
# This program is free software; you can redistribute it and/or
# modify it under the terms of the MIT License
#
# This library is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU GENERAL PUBLIC LICENSE for more details.
#

LIBYDER_LOCATION=./src
EXAMPLE_LOCATION=./examples
TEST_LOCATION=./test

all: release

debug:
	cd $(LIBYDER_LOCATION) && $(MAKE) debug $*

install:
	cd $(LIBYDER_LOCATION) && $(MAKE) install

clean:
	cd $(LIBYDER_LOCATION) && $(MAKE) clean
	cd $(EXAMPLE_LOCATION) && $(MAKE) clean
	cd $(TEST_LOCATION) && $(MAKE) clean

check:
	cd $(TEST_LOCATION) && $(MAKE) test

release:
	cd $(LIBYDER_LOCATION) && $(MAKE)

log_console:
	cd $(EXAMPLE_LOCATION) && $(MAKE) log_console

log_file:
	cd $(EXAMPLE_LOCATION) && $(MAKE) log_file

log_syslog:
	cd $(EXAMPLE_LOCATION) && $(MAKE) log_syslog

log_combined:
	cd $(EXAMPLE_LOCATION) && $(MAKE) log_combined

doxygen:
	doxygen doc/doxygen.cfg
