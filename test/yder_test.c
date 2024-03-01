/* Public domain, no copyright. Use at your own risk. */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <time.h>

#include <check.h>
#include <orcania.h>
#include "yder.h"

#define UNUSED(x) (void)(x)

char message[1024];
char app_name[128];
int level;

void unit_test_callback(void * cls, const char * log_app_name, const time_t date, const unsigned long log_level, const char * log_message) {
  UNUSED(date);
  strcpy(app_name, log_app_name);
  strcpy(cls, log_message);
  level = log_level;
}

void unit_test_newline(void * cls, const char * log_app_name, const time_t date, const unsigned long log_level, const char * log_message) {
  UNUSED(log_app_name);
  UNUSED(date);
  UNUSED(log_level);
  int * count = (int *)cls;
  ck_assert_int_gt(o_strlen(log_message), 0);
  (*count)++;
}

START_TEST(test_yder_init_console)
{
  ck_assert_int_eq(y_init_logs("test_yder_console", Y_LOG_MODE_CONSOLE, Y_LOG_LEVEL_DEBUG, NULL, "first test"), 1);
  y_close_logs();
}
END_TEST

START_TEST(test_yder_init_file)
{
  ck_assert_int_eq(y_init_logs("test_yder_file", Y_LOG_MODE_FILE, Y_LOG_LEVEL_DEBUG, "/tmp/test.log", "second test"), 1);
  y_close_logs();
}
END_TEST

START_TEST(test_yder_init_syslog)
{
#ifndef _WIN32
  ck_assert_int_eq(y_init_logs("test_yder_syslog", Y_LOG_MODE_SYSLOG, Y_LOG_LEVEL_DEBUG, NULL, "third test"), 1);
  y_close_logs();
#else
  ck_assert_int_eq(y_init_logs("test_yder_syslog", Y_LOG_MODE_SYSLOG, Y_LOG_LEVEL_DEBUG, NULL, "third test"), 0);
#endif
}
END_TEST

START_TEST(test_yder_init_journald)
{
#if !defined(_WIN32) && !defined(Y_DISABLE_JOURNALD)
  ck_assert_int_eq(y_init_logs("test_yder_journald", Y_LOG_MODE_JOURNALD, Y_LOG_LEVEL_DEBUG, NULL, "fourth test"), 1);
  y_close_logs();
#else
  ck_assert_int_eq(y_init_logs("test_yder_journald", Y_LOG_MODE_JOURNALD, Y_LOG_LEVEL_DEBUG, NULL, "fourth test"), 0);
#endif
}
END_TEST

START_TEST(test_yder_init_error_file)
{
  ck_assert_int_eq(y_init_logs("test_yder_file_fail", Y_LOG_MODE_FILE, Y_LOG_LEVEL_DEBUG, "/nope/nope", "second test"), 0);
  ck_assert_int_eq(y_init_logs("test_yder_file_fail", Y_LOG_MODE_FILE, Y_LOG_LEVEL_DEBUG, "", "second test"), 0);
  ck_assert_int_eq(y_init_logs("test_yder_file_fail", Y_LOG_MODE_FILE, Y_LOG_LEVEL_DEBUG, NULL, "second test"), 0);
}
END_TEST

START_TEST(test_yder_callback)
{
  message[0] = '\0';
  app_name[0] = '\0';
  char * app_name_orig = "test_yder_callback";
  ck_assert_int_eq(y_init_logs(app_name_orig, Y_LOG_MODE_CALLBACK, Y_LOG_LEVEL_DEBUG, NULL, "test_yder_callback"), 1);
  ck_assert_int_eq(y_set_logs_callback(NULL, NULL, NULL), 0);
  level = Y_LOG_LEVEL_NONE;
  ck_assert_int_eq(y_set_logs_callback(&unit_test_callback, message, "Start callback unit tests"), 1);
  ck_assert_str_eq(message, "Start callback unit tests");
  ck_assert_str_eq(app_name, app_name_orig);
  ck_assert_int_eq(level, Y_LOG_LEVEL_INFO);

  message[0] = '\0';
  app_name[0] = '\0';
  level = Y_LOG_LEVEL_NONE;
  y_log_message(Y_LOG_LEVEL_DEBUG, "first test");
  ck_assert_str_eq(message, "first test");
  ck_assert_str_eq(app_name, app_name_orig);
  ck_assert_int_eq(level, Y_LOG_LEVEL_DEBUG);

  message[0] = '\0';
  app_name[0] = '\0';
  level = Y_LOG_LEVEL_NONE;
  y_log_message(Y_LOG_LEVEL_DEBUG, "second test with parameter: %d", 42);
  ck_assert_str_eq(message, "second test with parameter: 42");
  ck_assert_str_eq(app_name, app_name_orig);
  ck_assert_int_eq(level, Y_LOG_LEVEL_DEBUG);

  message[0] = '\0';
  app_name[0] = '\0';
  level = Y_LOG_LEVEL_NONE;
  y_log_message(Y_LOG_LEVEL_DEBUG, "");
  ck_assert_str_eq(message, "");
  ck_assert_str_eq(app_name, app_name_orig);
  ck_assert_int_eq(level, Y_LOG_LEVEL_DEBUG);

  y_close_logs();
}
END_TEST

START_TEST(test_yder_init_multiple_error)
{
  ck_assert_int_eq(y_init_logs("test_yder_console", Y_LOG_MODE_CONSOLE, Y_LOG_LEVEL_DEBUG, NULL, "first test"), 1);
  ck_assert_int_eq(y_init_logs("test_yder_console", Y_LOG_MODE_CONSOLE, Y_LOG_LEVEL_DEBUG, NULL, "first test"), 0);
  y_close_logs();
}
END_TEST

START_TEST(test_yder_init_multiple_ok)
{
  ck_assert_int_eq(y_init_logs("test_yder_console", Y_LOG_MODE_CONSOLE, Y_LOG_LEVEL_DEBUG, NULL, "first test"), 1);
  ck_assert_int_eq(y_close_logs(), 1);
  ck_assert_int_eq(y_init_logs("test_yder_console", Y_LOG_MODE_CONSOLE, Y_LOG_LEVEL_DEBUG, NULL, "first test"), 1);
  ck_assert_int_eq(y_close_logs(), 1);
}
END_TEST

START_TEST(test_yder_init_close_multiple)
{
  ck_assert_int_eq(y_close_logs(), 1);
  ck_assert_int_eq(y_init_logs("test_yder_console", Y_LOG_MODE_CONSOLE, Y_LOG_LEVEL_DEBUG, NULL, "first test"), 1);
  ck_assert_int_eq(y_close_logs(), 1);
  ck_assert_int_eq(y_close_logs(), 1);
}
END_TEST

START_TEST(test_yder_level_debug)
{
  ck_assert_int_eq(y_init_logs("test_yder_level_debug", Y_LOG_MODE_CALLBACK, Y_LOG_LEVEL_DEBUG, NULL, "test_yder_level_debug"), 1);
  ck_assert_int_eq(y_set_logs_callback(&unit_test_callback, message, "Start callback unit tests"), 1);

  message[0] = '\0';
  y_log_message(Y_LOG_LEVEL_DEBUG, "first test");
  ck_assert_str_eq(message, "first test");

  message[0] = '\0';
  y_log_message(Y_LOG_LEVEL_INFO, "second test");
  ck_assert_str_eq(message, "second test");

  message[0] = '\0';
  y_log_message(Y_LOG_LEVEL_WARNING, "third test");
  ck_assert_str_eq(message, "third test");

  message[0] = '\0';
  y_log_message(Y_LOG_LEVEL_ERROR, "fourth test");
  ck_assert_str_eq(message, "fourth test");

  y_close_logs();
}
END_TEST

START_TEST(test_yder_level_info)
{
  ck_assert_int_eq(y_init_logs("test_yder_level_info", Y_LOG_MODE_CALLBACK, Y_LOG_LEVEL_INFO, NULL, "test_yder_level_info"), 1);
  ck_assert_int_eq(y_set_logs_callback(&unit_test_callback, message, "Start callback unit tests"), 1);

  message[0] = '\0';
  y_log_message(Y_LOG_LEVEL_DEBUG, "first test");
  ck_assert_str_eq(message, "");

  message[0] = '\0';
  y_log_message(Y_LOG_LEVEL_INFO, "second test");
  ck_assert_str_eq(message, "second test");

  message[0] = '\0';
  y_log_message(Y_LOG_LEVEL_WARNING, "third test");
  ck_assert_str_eq(message, "third test");

  message[0] = '\0';
  y_log_message(Y_LOG_LEVEL_ERROR, "fourth test");
  ck_assert_str_eq(message, "fourth test");

  y_close_logs();
}
END_TEST

START_TEST(test_yder_level_warning)
{
  ck_assert_int_eq(y_init_logs("test_yder_level_warning", Y_LOG_MODE_CALLBACK, Y_LOG_LEVEL_WARNING, NULL, "test_yder_level_warning"), 1);
  ck_assert_int_eq(y_set_logs_callback(&unit_test_callback, message, "Start callback unit tests"), 1);

  message[0] = '\0';
  y_log_message(Y_LOG_LEVEL_DEBUG, "first test");
  ck_assert_str_eq(message, "");

  message[0] = '\0';
  y_log_message(Y_LOG_LEVEL_INFO, "second test");
  ck_assert_str_eq(message, "");

  message[0] = '\0';
  y_log_message(Y_LOG_LEVEL_WARNING, "third test");
  ck_assert_str_eq(message, "third test");

  message[0] = '\0';
  y_log_message(Y_LOG_LEVEL_ERROR, "fourth test");
  ck_assert_str_eq(message, "fourth test");

  y_close_logs();
}
END_TEST

START_TEST(test_yder_level_error)
{
  ck_assert_int_eq(y_init_logs("test_yder_level_error", Y_LOG_MODE_CALLBACK, Y_LOG_LEVEL_ERROR, NULL, "test_yder_level_error"), 1);
  ck_assert_int_eq(y_set_logs_callback(&unit_test_callback, message, "Start callback unit tests"), 1);

  message[0] = '\0';
  y_log_message(Y_LOG_LEVEL_DEBUG, "first test");
  ck_assert_str_eq(message, "");

  message[0] = '\0';
  y_log_message(Y_LOG_LEVEL_INFO, "second test");
  ck_assert_str_eq(message, "");

  message[0] = '\0';
  y_log_message(Y_LOG_LEVEL_WARNING, "third test");
  ck_assert_str_eq(message, "");

  message[0] = '\0';
  y_log_message(Y_LOG_LEVEL_ERROR, "fourth test");
  ck_assert_str_eq(message, "fourth test");

  y_close_logs();
}
END_TEST

START_TEST(test_yder_date_format)
{
  ck_assert_int_eq(y_init_logs("test_yder_date_format", Y_LOG_MODE_CONSOLE, Y_LOG_LEVEL_DEBUG, NULL, "date format test"), 1);
  ck_assert_int_eq(y_set_date_format(NULL, "error"), 0);
  ck_assert_int_eq(y_set_date_format("TEST %Y - :%S", "success"), 1);
  y_log_message(Y_LOG_LEVEL_DEBUG, "first test");
  y_close_logs();
}
END_TEST

START_TEST(test_yder_newline_split)
{
  int count = 0;
  ck_assert_int_eq(y_init_logs("test_yder_newline_split", Y_LOG_MODE_CALLBACK, Y_LOG_LEVEL_DEBUG, NULL, "yder_newline_split"), 1);
  ck_assert_int_eq(y_set_logs_callback(&unit_test_newline, &count, "Start newline unit tests"), 1);
  ck_assert_int_eq(y_set_split_message_newline(1, "Set newline"), 1);
  count = 0;
  y_log_message(Y_LOG_LEVEL_DEBUG, "first test, one line");
  ck_assert_int_eq(count, 1);
  count = 0;
  y_log_message(Y_LOG_LEVEL_DEBUG, "second test\ntwo lines");
  ck_assert_int_eq(count, 2);
  count = 0;
  y_log_message(Y_LOG_LEVEL_DEBUG, "third test\nfour\nlines\nahead");
  ck_assert_int_eq(count, 4);
  y_close_logs();
}
END_TEST

static Suite *yder_suite(void)
{
  Suite *s;
  TCase *tc_core;

  s = suite_create("Yder tests functions");
  tc_core = tcase_create("test_yder");
  tcase_add_test(tc_core, test_yder_init_console);
  tcase_add_test(tc_core, test_yder_init_file);
  tcase_add_test(tc_core, test_yder_init_syslog);
  tcase_add_test(tc_core, test_yder_init_journald);
  tcase_add_test(tc_core, test_yder_init_error_file);
  tcase_add_test(tc_core, test_yder_init_multiple_error);
  tcase_add_test(tc_core, test_yder_init_multiple_ok);
  tcase_add_test(tc_core, test_yder_init_close_multiple);
  tcase_add_test(tc_core, test_yder_callback);
  tcase_add_test(tc_core, test_yder_level_debug);
  tcase_add_test(tc_core, test_yder_level_info);
  tcase_add_test(tc_core, test_yder_level_warning);
  tcase_add_test(tc_core, test_yder_level_error);
  tcase_add_test(tc_core, test_yder_date_format);
  tcase_add_test(tc_core, test_yder_newline_split);
  tcase_set_timeout(tc_core, 30);
  suite_add_tcase(s, tc_core);

  return s;
}

int main(void)
{
  int number_failed;
  Suite *s;
  SRunner *sr;

  s = yder_suite();
  sr = srunner_create(s);

  srunner_run_all(sr, CK_VERBOSE);
  number_failed = srunner_ntests_failed(sr);
  srunner_free(sr);

  return (number_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}
