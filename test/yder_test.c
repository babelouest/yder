/* Public domain, no copyright. Use at your own risk. */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <time.h>

#include <check.h>
#include "../include/yder.h"

START_TEST(test_yder)
{
  ck_assert_int_eq(y_init_logs("test_yder_console", Y_LOG_MODE_CONSOLE, Y_LOG_LEVEL_DEBUG, NULL, "first test"), 1);
  y_close_logs();
  ck_assert_int_eq(y_init_logs("test_yder_file", Y_LOG_MODE_FILE, Y_LOG_LEVEL_DEBUG, "/tmp/test.log", "second test"), 1);
  y_close_logs();
  ck_assert_int_eq(y_init_logs("test_yder_syslog", Y_LOG_MODE_SYSLOG, Y_LOG_LEVEL_DEBUG, NULL, "third test"), 1);
  y_close_logs();
  ck_assert_int_eq(y_init_logs("test_yder_journald", Y_LOG_MODE_JOURNALD, Y_LOG_LEVEL_DEBUG, NULL, "fourth test"), 1);
  y_close_logs();
  ck_assert_int_eq(y_init_logs("test_yder_file_fail", Y_LOG_MODE_FILE, Y_LOG_LEVEL_DEBUG, "/nope/nope", "second test"), 0);
}
END_TEST

static Suite *yder_suite(void)
{
	Suite *s;
	TCase *tc_core;

	s = suite_create("Yder tests functions");
	tc_core = tcase_create("test_yder");
	tcase_add_test(tc_core, test_yder);
	tcase_set_timeout(tc_core, 30);
	suite_add_tcase(s, tc_core);

	return s;
}

int main(int argc, char *argv[])
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
