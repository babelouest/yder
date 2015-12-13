# Yder

Logging library written in C.

Simple and easy to use logging library. You can log messages to the console, a file or syslog.

Yder is mono-thread, which mean that you can use only one instance of yder log at the same time in your program.

# Installation

Download yder from github repository, compile and install.

```shell
$ git clone https://github.com/babelouest/yder.git
$ cd yder/src
$ make
$ sudo make install
```

By default, the shared library and the header file will be installed in the `/usr/local` location. To change this setting, you can modify the `PREFIX` value in the `src/Makefile`.

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
