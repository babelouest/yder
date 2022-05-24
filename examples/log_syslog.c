/**
 * 
 * Yder example program
 * 
 * This example program describes the main features 
 * that are available in a callback function
 * 
 * Copyright 2014-2020 Nicolas Mora <mail@babelouest.org>
 * 
 * License MIT
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include "yder.h"

void write_logs(const char * level) {
  y_log_message(Y_LOG_LEVEL_ERROR, "This is an error message while level is %s", level);
  y_log_message(Y_LOG_LEVEL_WARNING, "This is a warning message while level is %s", level);
  y_log_message(Y_LOG_LEVEL_INFO, "This is an information message while level is %s", level);
  y_log_message(Y_LOG_LEVEL_DEBUG, "This is a debug message while level is %s", level);
}

int main(int argc, char ** argv) {
  
  if (y_init_logs("Yder Tests", Y_LOG_MODE_SYSLOG, Y_LOG_LEVEL_ERROR, NULL, "Initializing logs mode: syslog, logs level: error")) {
    write_logs("error");
    y_close_logs();
  }
  
  if (y_init_logs("Yder Tests", Y_LOG_MODE_SYSLOG, Y_LOG_LEVEL_WARNING, NULL, "Initializing logs mode: syslog, logs level: warning")) {
    write_logs("warning");
    y_close_logs();
  }
  
  if (y_init_logs("Yder Tests", Y_LOG_MODE_SYSLOG, Y_LOG_LEVEL_INFO, NULL, "Initializing logs mode: syslog, logs level: info")) {
    write_logs("info");
    y_close_logs();
  }
  
  if (y_init_logs("Yder Tests", Y_LOG_MODE_SYSLOG, Y_LOG_LEVEL_DEBUG, NULL, "Initializing logs mode: syslog, logs level: debug")) {
    write_logs("debug");
    y_close_logs();
  }
  
  return 0;
}
