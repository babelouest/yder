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
#include <string.h>
#include "yder.h"

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
  printf("Here is my callback log function\n- cls is %s\n- app_name is %s\n- date is %s\n- level is %s\n- message is '%s'\n\n", (char *)cls, app_name, date_stamp, level_name, message);
}

int main(int argc, char ** argv) {
  char * cls = "my cls";

  if (y_init_logs("Yder Tests", Y_LOG_MODE_CONSOLE, Y_LOG_LEVEL_DEBUG, NULL, "Initializing logs mode: console, logs level: debug")) {
    y_set_split_message_newline(1, "Set newline");
    y_log_message(Y_LOG_LEVEL_DEBUG, "A message in one line");
    y_log_message(Y_LOG_LEVEL_DEBUG, "A message in\ntwo lines");
    y_log_message(Y_LOG_LEVEL_DEBUG, "A message\nin\nfour\nlines");
    y_close_logs();
  }
  
  if (y_init_logs("Yder Tests", Y_LOG_MODE_FILE, Y_LOG_LEVEL_DEBUG, "/tmp/log_newline.log", "Initializing logs mode: file, logs level: debug")) {
    y_set_split_message_newline(1, "Set newline");
    y_log_message(Y_LOG_LEVEL_DEBUG, "A message in one line");
    y_log_message(Y_LOG_LEVEL_DEBUG, "A message in\ntwo lines");
    y_log_message(Y_LOG_LEVEL_DEBUG, "A message\nin\nfour\nlines");
    y_close_logs();
  }
  
  if (y_init_logs("Yder Tests", Y_LOG_MODE_JOURNALD, Y_LOG_LEVEL_DEBUG, NULL, "Initializing logs mode: journald, logs level: debug")) {
    y_set_split_message_newline(1, "Set newline");
    y_log_message(Y_LOG_LEVEL_DEBUG, "A message in one line");
    y_log_message(Y_LOG_LEVEL_DEBUG, "A message in\ntwo lines");
    y_log_message(Y_LOG_LEVEL_DEBUG, "A message\nin\nfour\nlines");
    y_close_logs();
  }
  
  if (y_init_logs("Yder Tests", Y_LOG_MODE_SYSLOG, Y_LOG_LEVEL_DEBUG, NULL, "Initializing logs mode: syslog, logs level: debug")) {
    y_set_split_message_newline(1, "Set newline");
    y_log_message(Y_LOG_LEVEL_DEBUG, "A message in one line");
    y_log_message(Y_LOG_LEVEL_DEBUG, "A message in\ntwo lines");
    y_log_message(Y_LOG_LEVEL_DEBUG, "A message\nin\nfour\nlines");
    y_close_logs();
  }
  
  if (y_init_logs("Yder Tests", Y_LOG_MODE_CALLBACK, Y_LOG_LEVEL_DEBUG, NULL, "Initializing logs mode: callback, logs level: warning") && y_set_logs_callback(&y_callback_log_message, cls, "callback init")) {
    y_set_split_message_newline(1, "Set newline");
    y_log_message(Y_LOG_LEVEL_DEBUG, "A message in one line");
    y_log_message(Y_LOG_LEVEL_DEBUG, "A message in\ntwo lines");
    y_log_message(Y_LOG_LEVEL_DEBUG, "A message\nin\nfour\nlines");
    y_close_logs();
  }
  
  if (y_init_logs("Yder Tests", Y_LOG_MODE_CONSOLE|Y_LOG_MODE_FILE|Y_LOG_MODE_CALLBACK, Y_LOG_LEVEL_DEBUG, "/tmp/log_newline_combined.log", "Initializing logs mode: file, logs level: debug") && y_set_logs_callback(&y_callback_log_message, cls, "callback init")) {
    y_set_split_message_newline(1, "Set newline");
    y_log_message(Y_LOG_LEVEL_DEBUG, "A message in one line");
    y_log_message(Y_LOG_LEVEL_DEBUG, "A message in\ntwo lines");
    y_log_message(Y_LOG_LEVEL_DEBUG, "A message\nin\nfour\nlines");
    y_close_logs();
  }
  
  return 0;
}
