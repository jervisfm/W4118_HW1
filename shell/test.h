/*
 * test.h
 *
 *  Created on: Sep 14, 2012
 */

#ifndef TEST_H_
#define TEST_H_

#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <assert.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <pwd.h>

/**
 * Tests
 */
void test_all(void);
void test_get_maximum_string(void);
void test_read_line(void);
void test_parse_line(void);
void test_initialize_string_array(void);
void test_is_builtin_command(void);
void test_can_execute_file(void);
void test_exists_file(void);
void test_join_path(void);
void test_get_first_path_index(void);
void test_add_string_to_path_list(void);
void test_remove_string_from_path_list(void);
void test_is_absolute_path(void);
void test_get_full_path(void);
void test_execv(void);
void test_free_pointer_array(void);
void test_run_path_cmd(void);
void test_add_string_to_history_list(void);
void test_combine_string_array(void);
void test_trim(void);
int test_print(void);
void test_delete_tail_from_list(void);

#endif /* TEST_H_ */
