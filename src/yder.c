/**
 * 
 * Yder Framework
 * 
 * Logging framework library
 * 
 * yder.h: structures and functions declarations
 * 
 * Copyright 2015-2016 Nicolas Mora <mail@babelouest.org>
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public License
 * as published by the Free Software Foundation;
 * version 2.1 of the License.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU GENERAL PUBLIC LICENSE for more details.
 *
 * You should have received a copy of the GNU General Public
 * License along with this library.  If not, see <http://www.gnu.org/licenses/>.
 * 
 */

#include "yder.h"

/**
 * Private functions declarations
 */
int   y_write_log(const char * app_name, const unsigned long init_mode, const unsigned long init_level, const char * init_log_file, const unsigned long level, const char * message);
void  y_write_log_console(const char * app_name, const time_t date, const unsigned long level, const char * message);
void  y_write_log_syslog(const char * app_name, const unsigned long level, const char * message);
void  y_write_log_file(const char * app_name, const time_t date, FILE * log_file, const unsigned long level, const char * message);

/**
 * Initialize logs
 */
int y_init_logs(const char * app, const unsigned long init_mode, const unsigned long init_level, const char * init_log_file, const char * message) {
  return y_write_log(app, init_mode, init_level, init_log_file, Y_LOG_LEVEL_INFO, message);
}

/**
 * Close logs
 */
int y_close_logs() {
  return y_write_log(NULL, 0, 0, NULL, 0, NULL);
}

/**
 * Write the message given in parameters to the current outputs if the current level matches
 */
void y_log_message(const unsigned long level, const char * message, ...) {
  va_list args, args_cpy;
  size_t out_len = 0;
  char * out = NULL;
  va_start(args, message);
  // Use va_copy to make a new args pointer to avoid problems with vsnprintf which can change args parameter on some architectures
  va_copy(args_cpy, args);
  out_len = vsnprintf(NULL, 0, message, args);
  out = malloc((out_len + 1)*sizeof(char));
  if (out != NULL) {
    vsnprintf(out, (out_len + 1), message, args_cpy);
    y_write_log(NULL, Y_LOG_MODE_CURRENT, Y_LOG_LEVEL_CURRENT, NULL, level, out);
    free(out);
  }
  va_end(args);
  va_end(args_cpy);
}

/**
 * Main function for logging messages
 * Warning ! Contains static variables used for not having to pass general configuration values every time you call log_message
 */
int y_write_log(const char * app_name, const unsigned long init_mode, const unsigned long init_level, const char * init_log_file, const unsigned long level, const char * message) {
  static unsigned long cur_mode = Y_LOG_MODE_NONE, cur_level = Y_LOG_LEVEL_NONE;
  FILE * cur_log_file = NULL;
  static char * cur_app_name = NULL;
  static const char * cur_log_file_path = NULL;
  time_t now;
  
  // Closing logs: free cur_app_name
  if (app_name == NULL && init_mode == Y_LOG_MODE_NONE && init_level == Y_LOG_LEVEL_NONE && init_log_file == NULL && level == Y_LOG_LEVEL_NONE && message == NULL) {
    free(cur_app_name);
    cur_app_name = NULL;
    return 1;
  }
  
  time(&now);
  
  if (init_mode != Y_LOG_MODE_CURRENT) {
    cur_mode = init_mode;
  }
  
  if (init_level != Y_LOG_LEVEL_CURRENT) {
    cur_level = init_level;
  }
  
  if (cur_mode == Y_LOG_MODE_NONE && cur_level == Y_LOG_LEVEL_NONE) {
    // Logs have not been initialized, cancel
    return 0;
  }

  if (init_log_file != NULL) {
    cur_log_file_path = init_log_file;
  }
  
  if (app_name != NULL) {
    cur_app_name = strdup(app_name);
  }
  
  if (cur_log_file_path != NULL) {
    if ((cur_log_file = fopen(cur_log_file_path, "a+")) == NULL) {
      perror("Error opening log file");
      return 0;
    }
  }
  
  // write message to expected output if level expected
  if (cur_level >= level) {
    if (message != NULL) {
      if (cur_mode & Y_LOG_MODE_CONSOLE) {
        y_write_log_console(cur_app_name, now, level, message);
      }
      if (cur_mode & Y_LOG_MODE_SYSLOG) {
        y_write_log_syslog(cur_app_name, level, message);
      }
      if (cur_mode & Y_LOG_MODE_FILE) {
        y_write_log_file(cur_app_name, now, cur_log_file, level, message);
      }
    }
  }
  
  if (cur_log_file != NULL && cur_log_file_path != NULL) {
    if (fclose(cur_log_file) != 0) {
      perror("Error closing log file");
      return 0;
    }
  }
  
  return 1;
}

/**
 * Write log message to console output (stdout or stderr)
 */
void y_write_log_console(const char * app_name, const time_t date, const unsigned long level, const char * message) {
  char * level_name = NULL, date_stamp[20];
  FILE * output = NULL;
  struct tm * tm_stamp;
  
  tm_stamp = localtime (&date);
  
  strftime (date_stamp, sizeof(date_stamp), "%FT%TZ", tm_stamp);
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
  if (level & Y_LOG_LEVEL_WARNING) {
    // Write to stderr
    output = stderr;
  } else {
    // Write to stdout
    output = stdout;
  }
  fprintf(output, "%s - %s %s: %s\n", date_stamp, app_name, level_name, message);
  fflush(output);
}

/**
 * Write log message to syslog
 */
void y_write_log_syslog(const char * app_name, const unsigned long level, const char * message) {
  openlog(app_name, LOG_PID|LOG_CONS, LOG_USER);
  switch (level) {
    case Y_LOG_LEVEL_ERROR:
      syslog( LOG_ERR, "%s", message );
      break;
    case Y_LOG_LEVEL_WARNING:
      syslog( LOG_WARNING, "%s", message );
      break;
    case Y_LOG_LEVEL_INFO:
      syslog( LOG_INFO, "%s", message );
      break;
    case Y_LOG_LEVEL_DEBUG:
      syslog( LOG_DEBUG, "%s", message );
      break;
  }
  closelog();
}

/**
 * Append log message to the log file
 */
void y_write_log_file(const char * app_name, const time_t date, FILE * log_file, const unsigned long level, const char * message) {
  char * level_name = NULL, date_stamp[20];
  struct tm * tm_stamp;
  
  if (log_file != NULL) {
    tm_stamp = localtime (&date);
    strftime (date_stamp, sizeof(date_stamp), "%Y-%m-%d %H:%M:%S", tm_stamp);
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
    fprintf(log_file, "%s - %s %s: %s\n", date_stamp, app_name, level_name, message);
    fflush(log_file);
  }
}
