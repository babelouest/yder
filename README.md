# Yder

Logging library written in C.

Simple and easy to use logging library. You can log messages to the console, a file, syslog or journald.

Yder is mono-thread, which mean that you can use only one instance of yder log at the same time in your program.

# Installation

### Distribution packages

[![Packaging status](https://repology.org/badge/vertical-allrepos/yder.svg)](https://repology.org/metapackage/yder)

Yder is available in multiple distributions as official package. Check out your distribution documentation to install the package automatically.

```shell
$ # Example for Debian testing
$ sudo apt install libyder-dev # Or apt install libyder2.0 if you don't need the development files
```

## Pre-compiled packages

You can install Yder with a pre-compiled package available in the [release pages](https://github.com/babelouest/yder/releases/latest/). Note that you need to install [Orcania](https://github.com/babelouest/orcania/releases/latest/) first. `jansson` development files packages is required to install Yder.

## Manual install

### Prerequisites

You must install [liborcania](https://github.com/babelouest/orcania) first before building libyder. Orcania will be automatically installed if missing and you're using cmake.

### CMake - Multi architecture

[CMake](https://cmake.org/download/) minimum 3.5 is required.

Run the cmake script in a subdirectory, example:

```shell
$ git clone https://github.com/babelouest/yder.git
$ cd yder/
$ mkdir build
$ cd build
$ cmake ..
$ make && sudo make install
```

The available options for cmake are:
- `-DWITH_JOURNALD=[on|off]` (default `on`): Build with journald (SystemD) support
- `-DBUILD_STATIC=[on|off]` (default `off`): Build the static archive in addition to the shared library
- `-DBUILD_TESTING=[on|off]` (default `off`): Build unit tests
- `-DINSTALL_HEADER=[on|off]` (default `on`): Install header file `yder.h`
- `-DCMAKE_BUILD_TYPE=[Debug|Release]` (default `Release`): Compile with debugging symbols or not

### Good ol' Makefile

Download yder from github repository, compile and install.

```shell
$ git clone https://github.com/babelouest/yder.git
$ cd yder/src
$ make
$ sudo make install
```

To build Yder without Journald (SystemD) support, add the option Y_DISABLE_JOURNALD=1 to the `make command`:

```shell
$ git clone https://github.com/babelouest/yder.git
$ cd yder/src
$ make Y_DISABLE_JOURNALD=1
$ sudo make install
```

By default, the shared library and the header file will be installed in the `/usr/local` location. To change this setting, you can modify the `DESTDIR` value in the `src/Makefile`.

Example: install yder in /tmp/lib directory

```shell
$ cd src
$ make && make DESTDIR=/tmp install
```

You can install Yder without root permission if your user has write access to `$(DESTDIR)`.
A `ldconfig` command is executed at the end of the install, it will probably fail if you don't have root permission, but this is harmless.
If you choose to install Yder in another directory, you must set your environment variable `LD_LIBRARY_PATH` properly.

### Install libyder as a static archive

Install  byderlibrary as a static archive, `libyder.a`, use the make commands `make static*`:

```shell
$ cd src
$ make static && sudo make static-install # or make DESTDIR=/tmp static-install if you want to install in `/tmp/lib`
```

# API Documentation

## Header files and compilation

To use yder in your code, you must include the file `yder.h`.

```c
#include <yder.h>
```

### Initialization

Use the `y_init_logs` function to start logging. The prototype of this function is:

```c
/**
 * Initialize logging with mode and level parameters, specify a log file if needed
 * Return true on success, false on error
 */
int y_init_logs(const char * app, const unsigned long init_mode, const unsigned long init_level, const char * init_log_file, const char * message);
```

The parameter `init_mode` is the initial mode for logging. You can specify and combine the following modes available:

```c
Y_LOG_MODE_CONSOLE
Y_LOG_MODE_SYSLOG
Y_LOG_MODE_JOURNALD
Y_LOG_MODE_FILE
```

If you use Y_LOG_MODE_FILE in your initial mode, you must specify a valid path for the `init_log_file` parameter.

The parameter `init_level` is the bottom level of your log messages. The levels available are, by level order:

```c
Y_LOG_LEVEL_NONE
Y_LOG_LEVEL_ERROR
Y_LOG_LEVEL_WARNING
Y_LOG_LEVEL_INFO
Y_LOG_LEVEL_DEBUG
```

For example, if you specify `Y_LOG_LEVEL_WARNING` as init_level, you will see in your log output only `Y_LOG_LEVEL_WARNING` and `Y_LOG_LEVEL_ERROR`. If you specify `Y_LOG_LEVEL_DEBUG`, you will see in your log output all log messages.

### Close yder

To close yder and free its allocated memory, use the function `y_close_logs`:

```c
/**
 * Close the logs
 */
int y_close_logs();
```

### Log a message

To log a message, use the function `y_log_message`, defined by:

```c
/**
 * Log a message using current parameters
 */
void y_log_message(const unsigned long type, const char * message, ...);
```

This function uses `printf` prototype for the message and the log message type. For example:

```c
y_log_message(Y_LOG_LEVEL_INFO, "Initializing application");
y_log_message(Y_LOG_LEVEL_ERROR, "Error in application, you have %d over %d threads in error mode", threads_error, threads_total);
```

### Example source code

See `examples` folder for detailed sample source codes.
