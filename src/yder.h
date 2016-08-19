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

#ifndef __YDER_H__
#define __YDER_H__

#include <stdarg.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <syslog.h>

#define YDER_VERSION 1.0

#define Y_LOG_MODE_NONE    0x0000
#define Y_LOG_MODE_CONSOLE 0x00F0
#define Y_LOG_MODE_SYSLOG  0x0F00
#define Y_LOG_MODE_FILE    0xF000
#define Y_LOG_MODE_CURRENT 0xFFFF

#define Y_LOG_LEVEL_NONE    0x0000
#define Y_LOG_LEVEL_ERROR   0x000F
#define Y_LOG_LEVEL_WARNING 0x00F0
#define Y_LOG_LEVEL_INFO    0x0F00
#define Y_LOG_LEVEL_DEBUG   0xF000
#define Y_LOG_LEVEL_CURRENT 0xFFFF

/**
 * Initialize logging with mode and level parameters, specify a log file if needed
 * Return true on success, false on error
 */
int y_init_logs(const char * app, const unsigned long init_mode, const unsigned long init_level, const char * init_log_file, const char * message);

/**
 * Close the logs
 */
int y_close_logs();

/**
 * Log a message using current parameters
 */
void y_log_message(const unsigned long type, const char * message, ...);

#endif // __YDER_H__
