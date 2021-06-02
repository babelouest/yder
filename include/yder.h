/**
 * 
 * @file yder.h
 * @brief Yder framework
 * 
 * Logging framework library
 * 
 * yder.h: structures and functions declarations
 * 
 * Copyright 2015-2020 Nicolas Mora <mail@babelouest.org>
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

#ifndef __YDER_H__
#define __YDER_H__

#ifdef __cplusplus
extern "C"
{
#endif

#include "yder-cfg.h"

#include <time.h>

/**
 * @defgroup const Constants
 * @{
 */

#define Y_LOG_MODE_NONE     0x00000000
#define Y_LOG_MODE_CONSOLE  0x000000F0
#define Y_LOG_MODE_SYSLOG   0x00000F00
#define Y_LOG_MODE_FILE     0x0000F000
#define Y_LOG_MODE_JOURNALD 0x000F0000
#define Y_LOG_MODE_CALLBACK 0x00F00000
#define Y_LOG_MODE_CURRENT  0xFFFFFFFF

#define Y_LOG_LEVEL_NONE    0x0000
#define Y_LOG_LEVEL_ERROR   0x000F
#define Y_LOG_LEVEL_WARNING 0x00F0
#define Y_LOG_LEVEL_INFO    0x0F00
#define Y_LOG_LEVEL_DEBUG   0xF000
#define Y_LOG_LEVEL_CURRENT 0xFFFF

/**
 * @}
 */

/**
 * @defgroup init Initialization and closing functions
 * @{
 */

/**
 * Initialize logging with mode and level parameters, specify a log file if needed
 * @param app: the name of the application that will be included in all log messages
 * @param init_mode: log mode, values available are
 * Y_LOG_MODE_NONE, Y_LOG_MODE_CONSOLE, Y_LOG_MODE_SYSLOG, Y_LOG_MODE_FILE, Y_LOG_MODE_JOURNALD
 * Multiple log modes are possible, values must be stacked, ex: Y_LOG_MODE_SYSLOG|Y_LOG_MODE_JOURNALD
 * @param init_level: minimum level of the log messages
 * Values available are Y_LOG_LEVEL_NONE, Y_LOG_LEVEL_ERROR, Y_LOG_LEVEL_WARNING, Y_LOG_LEVEL_INFO or Y_LOG_LEVEL_DEBUG
 * @param init_log_file: path to the file that will contain the log messages if log mode Y_LOG_MODE_FILE is selected
 * @param message: first message that will be appear in the logs
 * @return 1 on success, 0 on error
 */
int y_init_logs(const char * app, const unsigned long init_mode, const unsigned long init_level, const char * init_log_file, const char * message);

/**
 * Specify a callback function that will catch all log messages
 * In addition to other logs output already defined in y_init_logs
 * @param y_callback_log_message a pointer to a user-defined callback function
 * @param cls a user-defined pointer that will be available on y_callback_log_message
 * @param message message that will be appear in the logs
 * @return 1 on success, 0 on error
 */
int y_set_logs_callback(void (* y_callback_log_message) (void * cls, const char * app_name, const time_t date, const unsigned long level, const char * message),
                        void * cls,
                        const char * message);

/**
 * Specify a date format for console and file logging
 * Uses strftime syntax
 * @param format date format using strftime syntax
 * @param message message that will be appear in the logs
 * @return 1 on success, 0 on error
 */
int y_set_date_format(const char * format, const char * message);

/**
 * Close the logs
 * @return 1 on success, 0 on error
 */
int y_close_logs();

/**
 * @}
 */

/**
 * @defgroup log Logging function
 * @{
 */

/**
 * Log a message using current parameters
 * @param type type of the log message
 * Values available are Y_LOG_LEVEL_ERROR, Y_LOG_LEVEL_WARNING, Y_LOG_LEVEL_INFO or Y_LOG_LEVEL_DEBUG
 * @param message the message to log, using printf format
 */
void y_log_message(const unsigned long type, const char * message, ...);

/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif // __YDER_H__
