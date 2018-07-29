#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "../include/yder.h"

void y_callback_log_message(void * cls, const char * app_name, const time_t date, const unsigned long level, const char * message) {
  char *level_name, date_stamp[20];
  struct tm * tm_stamp;
  
  tm_stamp = localtime (&date);
  
#ifndef _WIN32
  strftime (date_stamp, sizeof(date_stamp), "%FT%TZ", tm_stamp);
#else
  strftime (date_stamp, sizeof(date_stamp), "%Y-%m-%dT%H:%M:%S", tm_stamp);
#endif
  switch (level) {
    case Y_LOG_LEVEL_ERROR:
      level_name = "ERROR";
      break;
    case Y_LOG_LEVEL_WARNING:
      level_name = "WARNING";
      break;
    case Y_LOG_LEVEL_INFO:
      level_name = "INFO";
      break;
    case Y_LOG_LEVEL_DEBUG:
      level_name = "DEBUG";
      break;
    default:
      level_name = "NONE";
      break;
  }
  printf("Here is my callback log function\n- app_name is %s\n- date is %s\n- level is %s\n- message is '%s'\n\n", app_name, date_stamp, level_name, message);
}

void write_logs(const char * level) {
  y_log_message(Y_LOG_LEVEL_ERROR, "This is an error message while level is %s", level);
  y_log_message(Y_LOG_LEVEL_WARNING, "This is a warning message while level is %s", level);
  y_log_message(Y_LOG_LEVEL_INFO, "This is an information message while level is %s", level);
  y_log_message(Y_LOG_LEVEL_DEBUG, "This is a debug message while level is %s", level);
}

int main(int argc, char ** argv) {
  char * level = NULL;
  char * cls = "my cls";
  
  y_log_message(Y_LOG_LEVEL_ERROR, "This is an test error message without initialized logs");
  
  if (y_init_logs("Yder Tests", Y_LOG_MODE_CALLBACK, Y_LOG_LEVEL_ERROR, NULL, "Initializing logs mode: callback, logs level: error") && y_set_logs_callback(&y_callback_log_message, cls, "callback init")) {
    level = "error";
    write_logs(level);
    y_close_logs();
  }
  
  if (y_init_logs("Yder Tests", Y_LOG_MODE_CALLBACK, Y_LOG_LEVEL_WARNING, NULL, "Initializing logs mode: callback, logs level: warning") && y_set_logs_callback(&y_callback_log_message, cls, "callback init")) {
    level = "warning";
    write_logs(level);
    y_close_logs();
  }
  
  if (y_init_logs("Yder Tests", Y_LOG_MODE_CALLBACK, Y_LOG_LEVEL_INFO, NULL, "Initializing logs mode: callback, logs level: info") && y_set_logs_callback(&y_callback_log_message, cls, "callback init")) {
    level = "info";
    write_logs(level);
    y_close_logs();
  }
  
  if (y_init_logs("Yder Tests", Y_LOG_MODE_CALLBACK, Y_LOG_LEVEL_DEBUG, NULL, "Initializing logs mode: callback, logs level: debug") && y_set_logs_callback(&y_callback_log_message, cls, "callback init")) {
    level = "debug";
    write_logs(level);
    y_close_logs();
  }
  
  return 0;
}
