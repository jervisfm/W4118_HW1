/*
 * test.c
 *  Created on: Sep 14, 2012
 *  Unit Tests for Shell.c Simple Shell.
 */

#include "test.h"
#include "shell.h"

int main(void) {
	test_all();
	return 0;
}

/***** TESTS *******/
void test_all(void) {
	printf("Running Tests...\n");
	test_trim();
	test_combine_string_array();
	test_add_string_to_history_list();
	test_run_path_cmd();
	test_free_pointer_array();
	test_get_full_path();
	test_is_absolute_path();
	test_add_string_to_path_list();
	test_remove_string_from_path_list();
	test_get_first_path_index();
	test_join_path();
	test_exists_file();
	test_can_execute_file();
	test_is_builtin_command();
	test_parse_line();
	test_get_maximum_string();
	test_initialize_string_array();
	printf("\n*** ALL Tests Passed! ***** \n");
}

int test_print(void) {
	printf(TEXT);
	return 0;
}
void test_get_maximum_string(void) {
	char test[] = " This is a test string ";
	char test2[]= "This is a VeryLongTestString";

	const int expected_max1 = 6;
	const int expected_max2 = 18;

	int actual_max1 = get_maximum_string(test);
	int actual_max2 = get_maximum_string(test2);
	assert(expected_max1 == actual_max1);
	printf("expected max2 == %d", expected_max2);
	printf("\nactual max2 == %d", actual_max2);
	assert(expected_max2 == actual_max2);
}

void test_initialize_string_array(void) {
	const int buffer_size = 6;
	const int array_size = 3;
	char* sbuf[array_size];
	initialize_string_array(sbuf, buffer_size, array_size);
	char* test[] = {"one", "two", "three"};
	int i = 0;
	for(i = 0; i < array_size; ++i) {
		sbuf[i] = test[i];
		assert(strcasecmp(sbuf[i], test[i]) == 0);
	}
}

void test_read_line() {
	char* c = read_line();
	printf("\nWe read: \n%s\n", c);
	return;
}

void test_parse_line(void) {
	char test[] = "This is a test string 6 7 8 9 10 11 12";
	//char test2[] = "One Two Three Four Five Six Seven Eight Nine Ten Elven";

	char* parsed[MAX_ARGUMENTS];
	parse_line(test, parsed, MAX_ARGUMENTS);
	//parse_line(test2,parsed, MAXIMUM_ARGUMENTS);
	/* test */
	assert(strcasecmp(parsed[0], "This") == 0);
	assert(strcasecmp(parsed[1], "is") == 0);
	assert(strcasecmp(parsed[2], "a") == 0);
	assert(strcasecmp(parsed[3], "test") == 0);
	assert(strcasecmp(parsed[4], "string") == 0);
	assert(strcasecmp(parsed[5], "6") == 0);
	assert(strcasecmp(parsed[6], "7") == 0);
	assert(strcasecmp(parsed[7], "8") == 0);
	assert(strcasecmp(parsed[8], "9") == 0);
	assert(strcasecmp(parsed[9], "10") == 0);
	assert(strcasecmp(parsed[10], "11") == 0);
}

void test_is_builtin_command(void) {
	char * cmd1 = "CD";
	char* cmd2 = "cd";
	assert(is_builtin_command(cmd1) == 0);
	assert(is_builtin_command(cmd2) == 1);
}

void test_can_execute_file(void) {
	char * test = "/sbin/dhclient";
	assert(can_execute_file(test) == 1);
}

void test_exists_file(void) {
	char * test = "/sbin/dhclient";
	assert(exists_file(test) == 1);
}


void test_join_path(void) {
	char dir1[] = "/sbin/";
	char dir2[] = "/sbin";
	char path[] = "dhclient";
	char expected[] = "/sbin/dhclient";
	assert(strcmp(expected, join_path(dir1, path)) == 0);
	assert(strcmp(expected, join_path(dir2, path)) == 0);
}

void test_get_first_path_index(void) {
	const char* paths[] =  {"/tmp", "/home", "/sbin"} ;
	char cmd[]="dhclient";
	int array_size = 3;
	int index = get_first_path_index(cmd, paths, array_size);
	assert(index == 2);
}

void test_add_string_to_path_list(void) {
	init();
	add_string_to_path_list("one", &PATH);
	add_string_to_path_list("two", &PATH);
	add_string_to_path_list("three", &PATH);

	assert(strcmp(PATH.head->data, "three") == 0);
	assert(PATH.head->size == 6);
	assert(strcmp(PATH.head->next->data, "two") == 0);
	assert(PATH.head->next->size == 4);
	assert(strcmp(PATH.head->next->next->data, "one") == 0);
	assert(PATH.head->next->next->size == 4);
	assert(PATH.size == 3);
}

void test_remove_string_from_path_list(void) {
	init();
	add_string_to_path_list("one", &PATH);
	add_string_to_path_list("two", &PATH);
	add_string_to_path_list("three", &PATH);

	remove_string_from_path_list("two", &PATH);

	assert(strcmp(PATH.head->data, "three") == 0);
	assert(PATH.head->size == 6);
	assert(strcmp(PATH.head->next->data, "one") == 0);
	assert(PATH.head->next->size == 4);
	assert(PATH.size == 2);
}

void test_is_absolute_path(void) {
	char test[] = "    /abs/test/yes";
	char test2[] = "    no/relative";
	assert(is_absolute_path(test) == 1);
	assert(is_absolute_path(test2) == 0);
}

void test_get_full_path(void) {
	init();
	add_string_to_path_list(".", &PATH);
	char dir[] = "/bin";
	char path1[] = "ls";
	char expected1[] = "/bin/./ls";

	char path2[] = "/bin/du";
	char expected2[] = "/bin/du";
	chdir(dir);
	char* result1 = get_full_path(path1);
	char* result2 = get_full_path(path2);
	assert(strcmp(result1,expected1) == 0);
	assert(strcmp(result2, expected2) == 0);
}

void test_execv(void) {
	printf("Running command...\n");
	char cmd[] = "/bin/ls";
	char* params[3];
	params[0] = cmd;
	params[1] = 0;
	//params[1] = NULL;
	int pid;
	if((pid = fork())) {
		int  i;
		while(wait(&i) != pid);
	} else {
		execv(cmd, params);
		printf("%c", (int)params[0]);
		perror("An error occurred while execing");
		exit(-1);
	}
}

void test_free_pointer_array(void) {
	char** test = (char**) calloc(2, sizeof(char*));
	int size =2;
	int  i =0;
	for(; i < size; ++i) {
		test[i] = calloc(4, sizeof(char));
		strncat(test[i],"two" ,4 );
	}
	free_pointer_array((void**)test, size, 0);
	assert(1);
}

void test_run_path_cmd(void) {
	const char* test1[] = {"path", "+", "ONE"};
	const char* test2[] = {"path", "+","TWO"};
	const char* test3[] = {"path", "+", "THREE"};
	const char* test4[] = {"path", "-","TWO"};
	run_path_cmd(test1);
	run_path_cmd(test2);
	run_path_cmd(test3);

	assert(strcmp(PATH.head->data, "THREE") == 0);
	assert(strcmp(PATH.head->next->data, "TWO") == 0);
	assert(strcmp(PATH.head->next->next->data, "ONE") == 0);
	run_path_cmd(test4);
	printf("\n\n%s\n", PATH.head->next->data);
	assert(strcmp(PATH.head->data, "THREE") == 0);
	assert(strcmp(PATH.head->next->data, "ONE") == 0);
}

void test_add_string_to_history_list(void) {
	init();
	char test[5];
	const int REPEAT = 200;
	const int HISTORY_SIZE = 100;
	int i = 0;
	printf("got here 1\n");

	for(; i < REPEAT; ++i){
		snprintf(test, 4, "%d", i);
		add_string_to_history_list((const char*)test, &HISTORY);
	}
	printf("got here 3\n");

	/*verify results */
	assert(HISTORY.size == 100);
	struct String* curr = HISTORY.head;
	int counter = 100;
	for(i = 0; i < HISTORY_SIZE; ++i, ++counter) {
		snprintf(test, 4, "%d", counter);
		assert(strcmp(curr->data, test) == 0);
		curr = curr->next;
	}
}
void test_combine_string_array(void) {
	const char* test[] = {"one", "two", "three"};
	int size  = 3;
	char expected[] = "one two three";
	char* combined = combine_string_array(test,size);
	printf("\nGot:%s", combined);
	assert(strcmp(combined, expected) == 0);
}

void test_trim(void) {
	char test[] = "   hello   ";
	char expected[] = "hello";
	trim_whitespace(test);
	assert(strcmp(test, expected) == 0);
	trim_whitespace(test);
	assert(strcmp(test, expected) == 0);
}
