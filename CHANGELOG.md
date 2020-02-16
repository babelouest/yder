# Yder Changelog

## 2.0.0

- Improve thread-safetyness

## 1.4.9

- Fix pkgconfig dependency
- Add doxygen documentation

## 1.4.8

- Avoid running `y_init_logs` without closing between

## 1.4.7

- Fix build for MacOSX (thanks JohnAZoidberg!)
- Clean build process
- Clean static variables if error initializing logs with file

## 1.4.6

- Fix package dependencies in cmake script

## 1.4.5

- Install pkgconfig file when using Makefile
- Update src/Makefile to show more information
- Adapt for MSVC
- Fix pkgconfig bugs

## 1.4.4

- Improve build config file and install headers

## 1.4.3

- Add config file yder-cfg.h dynamically built with the options

## 1.4.2

- Add Travis CI
- Change cmake option BUILD_TESTING to BUILD_YDER_TESTING
- Add RPM in CMake script package

## 1.4.1

- Fix a bug about including time.h in the wrong place

## 1.4.0

- Add y_init_callback_logs to redirect all logs in a callback function

## 1.3.4

- Fix CMake build when /usr/local is not present in default build path

## 1.3.3

- Fix Makefile soname

## 1.3.2

- Fix Debian package build on CMake script

## 1.3.1

- Add Debian hardening patch on Makefile

## 1.3

- Fix Syslog bug
- Add Journald (SystemD) logs with the mode Y_LOG_MODE_JOURNALD

## 1.2.1

- Update Orcania version
- Added flags for building local static libraries

## 1.2.0

- Add CMake installation script
