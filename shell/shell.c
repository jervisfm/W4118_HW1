
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <assert.h>
#include <string.h>
#include <unistd.h>
#include "shell.h"


int test (int argc, char** argv)
{


	return 0;

}


int main(int argc, char **argv)
{
	//test(argc, argv);
	test_all();
	return 0;
}




/*
 * Reads line from stdin
 * Caller responsible for freeing returned line string
 */
char* read_line()
{
	char c = '\0';
	char* line = (char *) calloc(1, sizeof(char));

	/*memory allocation failed*/
	if(line == NULL) {
		print_error("ReadLine memory allocation failed");
		return NULL;
	}

	/*current index into line*/
	int i = 0;

	/* this includes the null character.
	 * so an empty string has size of 1
	 */
	int string_size = 1;

	while (scanf("%c", &c) > 0 && c != '\n') {
		++string_size;
		int new_buffersize = sizeof(char) * (string_size);
		line = realloc(line, new_buffersize);
		if(line == NULL) {
			print_error("ReadLine memory reallocation failed");
			return NULL;
		}
		line[i] = c;
		line[i+1] = '\0';
		++i;
	}
	if(c == '\0') {
		print_error("Reading character from std failed");
		return NULL;
	}
	return line;
}

/**
 * This parses the given line into separate string.
 * The delimeter used by default is whitespace.
 * Size is the maximum number of different strings to parse
 */
void parse_line(const char* line, char* parsed[], const int size)
{
	int buffer_size = get_maximum_string(line) + 1;
	printf("Max Buffer : %d\n", buffer_size);
	initialize_string_array(parsed, buffer_size, MAXIMUM_ARGUMENTS);
	int string_no = 0;

	int char_idx = 0;
	enum ParseMode mode = reading;
	char current = '\0';
	char next = '\0';
	int i = 0;

	for(; line[i] != '\0'; ++i) {
		current = line[i];
		next = line[i + 1];
		if(isspace(current)) {
			mode = skipping;
		} else {
			mode = reading;
		}


		if(string_no >= MAXIMUM_ARGUMENTS) {
			printf("Only %d arguments were considered",
				MAXIMUM_ARGUMENTS);
			break;
		}

		switch (mode) {
			case reading:
			{
				char* string = parsed[string_no];
				string[char_idx++] = current;

				if (isspace(next)) {
					string[char_idx] = '\0';
					++string_no;
				}
				break;
			}
			case skipping:
			{
				char_idx = 0;
				break;
			}

			default:
				break;
		}
	}
}

/**
 * Allocates and initializes a string array.
 * 'array' - is the string array to intialize
 * 'buffer_size' is the size of the buffer to allocate for each string
 * 'array_size' total number of elements in the array.
 *
 * Note that pass by value is Okay since the array elements are pointers.
 */
void initialize_string_array(char* array[], int buffer_size, int array_size) {
	int i = 0;
	printf("%d", buffer_size);

	for(i = 0; i < array_size; ++i) {
		array[i] = (char*) calloc(buffer_size, sizeof(char));
		if(array[i] == NULL) {
			print_error("Memory allocation failed in "
				    "initialize_string_array");
			exit(EXIT_FAILURE);
		}
	}
}

int get_maximum_string(const char* line) {
	int i = 0;
	int max = 0;
	int size = 0;
	enum ParseMode mode = reading;
	char current = '\0';
	char next = '\0';
	for(; line[i] != '\0'; ++i) {
		current = line[i];
		next = line[i + 1];
		if(isspace(current)) {
			mode = skipping;
		} else {
			mode = reading;
		}

		switch (mode) {
			case reading:
			{
				++size;
				if (size > max) {
					max = size;
				}
				break;
			}
			case skipping:
			{
				size = 0;
				break;
			}

			default:
				break;
		}
	}
	return max;
}

void print_prompt() {
	printf("$ ");
}

void print_error(const char* err) {
	printf("error: %s", err);
}

/* Checks if given command is a builtin command.
 * Returns 1 if true and 0 if false. */
int is_builtin_command(const char* cmd) {
	int i = 0;
	for(i = 0; i < BUILTIN_COMMANDS_SIZE; ++i) {
		if(strcmp(BUILTIN_COMMANDS[i], cmd) == 0)
			return 1;
	}
	return 0;
}


int run_command(const char* cmd[], int array_size) {
//	char* command = cmd[0];
//	int params_size = array_size - 1;
//	char** params = &cmd[1];
//	int pid;
//	pid = fork();
//	if(pid < 0) { /* an error occured */
//		print_error("Forking failed - Cannot execute given command.");
//		return -1;
//	} else if (pid == 0 ) { /*this is the child process*/
//		execv(command, params);
//	} else { /* pid > 0 :  i.e. parent process*/
//
//	}

	return 0;

}

/**
 * Determines if the given file path exists or not.
 * return 1 on success and 0 on failure
 */
int exists_file(const char* file_path) {
	/* access returns 0 on success */
	if (access(file_path, R_OK) == 0) {
		return 1;
	} else {
		return 0;
	}
}

/**
 * Determines if the given file can be ran
 * return 1 on success and 0 on failure
 */
int can_execute_file(const char* file_path) {
	/* access returns 0 on success */
	if (access(file_path, X_OK) == 0) {
		return 1;
	} else {
		return 0;
	}
}

/*
 * Returns the index of the first path for which the given
 * cmd is found.
 * Otherwise returns -1 to indicate cmd was not found in any path
 */
int get_first_path_index(const char* cmd, const char* paths[], int array_size) {
	int i = 0;
	for(; i < array_size; ++i) {

	}
	return -1;
}

/**
 * Joins together the given directory with the specified path.
 * Caller is responsible for freeing return string.
 */
char* join_path(const char* dir, const char* path) {
	int i = 0;
	char curr;
	char next;
	/* we add 3 to account for null characters in dir & path
	 * and also to account for possibility of having a "/" in the dir
	 * string
	 */
	int combined_size = strlen(dir) + strlen(path) + 3;
	char * joined = calloc(combined_size, sizeof(char));
	check_allocated_mem("join_path", joined);

	for(; dir[i] != '\0'; ++i) {
		curr = dir[i];
		next = dir[i+1];
		if(next == '\0') { /*we currently at the last character*/
			if (curr != '/') {
				strcat(joined, dir);
				strcat(joined, "/");
				strcat(joined, path);
			} else {
				strcat(joined, dir);
				strcat(joined, path);
			}
		}
	}
	return joined;
}


/*
 * Checks the given pointer to ensure that it has been allocated
 * properly (i.e. not null). If null, it exits the program.
 * function - is the name of the function in which the ptr was allocated.
 */
void check_allocated_mem(const char* function, void * input) {
	if(input == NULL) {
		char err_msg[] = "Memory Allocation Failure in ";
		int size = strlen(function) + 1 + 30;
		char err[size];
		memset(&err, 0, size);
		strcat(err, err_msg);
		strcat(err, function);
		print_error(err);
		exit(EXIT_FAILURE);
	}
}


/***** TESTS *******/
void test_all(void) {
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

	char* parsed[MAXIMUM_ARGUMENTS];
	parse_line(test, parsed, MAXIMUM_ARGUMENTS);
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

