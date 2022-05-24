# Yder

Logging library written in C.

Simple and easy to use logging library. You can log messages to the console, a file, syslog, journald or a callback function.

Yder is multi-thread, which mean that you can use multiple instances of the function `y_log_message` at the same time in your program.

Yet, `y_init_logs`, `y_set_logs_callback`, `y_set_date_format`, `y_set_split_message_newline` and `y_close_logs` are mono-thread. You might use those functions in the `main()` routine to ensure safety.
