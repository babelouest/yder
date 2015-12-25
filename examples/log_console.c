#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../src/yder.h"

void write_logs(const char * level) {
  y_log_message(Y_LOG_LEVEL_ERROR, "This is an error message while level is %s", level);
  y_log_message(Y_LOG_LEVEL_WARNING, "This is a warning message while level is %s", level);
  y_log_message(Y_LOG_LEVEL_INFO, "This is an information message while level is %s", level);
  y_log_message(Y_LOG_LEVEL_DEBUG, "This is a debug message while level is %s", level);
}

int main(int argc, char ** argv) {
  char * level = NULL;
  
  y_log_message(Y_LOG_LEVEL_ERROR, "This is an test error message without initialized logs");
  
  if (y_init_logs("Yder Tests", Y_LOG_MODE_CONSOLE, Y_LOG_LEVEL_ERROR, NULL, "Initializing logs mode: console, logs level: error")) {
    level = "error";
    write_logs(level);
    y_close_logs();
  }
  
  if (y_init_logs("Yder Tests", Y_LOG_MODE_CONSOLE, Y_LOG_LEVEL_WARNING, NULL, "Initializing logs mode: console, logs level: warning")) {
    level = "warning";
    write_logs(level);
    y_close_logs();
  }
  
  if (y_init_logs("Yder Tests", Y_LOG_MODE_CONSOLE, Y_LOG_LEVEL_INFO, NULL, "Initializing logs mode: console, logs level: info")) {
    level = "info";
    write_logs(level);
    y_close_logs();
  }
  
  if (y_init_logs("Yder Tests", Y_LOG_MODE_CONSOLE, Y_LOG_LEVEL_DEBUG, NULL, "Initializing logs mode: console, logs level: debug")) {
    level = "debug";
    write_logs(level);
    y_close_logs();
  }
  
  return 0;
}
