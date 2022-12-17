# Yder Changelog

## 1.4.19

- Improve cmake script and MSVC support
- Build with flag `-Wconversion`

## 1.4.18

- Minor bugfixes

## 1.4.17

- Fix `CMAKE_MODULE_PATH` who was used as single value (for real)

## 1.4.16

- Fix pkg-config file with absolute path for `CMAKE_INSTALL_{INCLUDE,LIB}DIR`
- Fix `CMAKE_MODULE_PATH` who was used as single value
- Add `y_set_split_message_newline` to split log messages on newline characters

## 1.4.15

- Use `o_strnullempty` instead of `o_strlen` to check if a `char *` has data

## 1.4.14

- Add ld flags in libyder.pc on static build

## 1.4.13

- Use `gmtime_s` on Windows (Thanks sudhip)

## 1.4.12

- Use `gmtime_r` instead of `localtime`
- Fix documentation typos

## 1.4.11

- Update CI pipelines
- cmake script: Add option `DOWNLOAD_DEPENDENCIES`
- Harmonize licenses
- Fix install pkgconfig by Makefile

## 1.4.10

- Add `y_set_date_format`, closes #17
- Remove generated doc from source package

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
