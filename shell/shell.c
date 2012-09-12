
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <assert.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <pwd.h>
#include "shell.h"


int test (int argc, char** argv)
{
	return 0;

}


int main(int argc, char **argv)
{
	//test(argc, argv);
	//test_all();
	run_shell();
	return 0;
}


/* Executes a very basic shell
 * This is a simple shell that can has ls path and history
 * commands builtin. Type exit to quit and close the shell.
 */
int run_shell(void) {
	init();
	while(1) {
		print_prompt();
		char* user_input = read_line();
		char* parsed[MAXIMUM_ARGUMENTS];
		parse_line(user_input, parsed, MAXIMUM_ARGUMENTS);
		run_command((const char**)parsed, MAXIMUM_ARGUMENTS);
		free(user_input);
	}
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
	/* printf("Max Buffer : %d\n", buffer_size); */
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
			printf("Only considering %d arguments\n",
				MAXIMUM_ARGUMENTS - 1);
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
	printf("\n$ ");
}

void print_error(const char* err) {
	printf("error: %s\n", err);
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


/**
 * Parses the given command and returns back an array of
 * parameters suitable for passing to the execv system call.
 * By convention, first argument of params is the name of the
 * program being run w/o any directory paths.
 *
 * Also, immendiately after the last argument, there will be
 * a NULL pointer. This is the format that the execv command expects.
 *
 * Size of the return array is set in the parameter 'param_size'
 *
 * Caller is responsible for freeing returned array.
 *
 */
char* const* get_params(const char* cmd[], int array_size, int* param_size) {
	/* Duplicate given command with its parameters
	 * but leave space for one extra NULL element at the end*/
	char** copy = calloc(array_size + 1, sizeof(char*));
	copy[array_size] = NULL;
	int i = 0;
	*param_size = 0;
	for(; i < array_size; ++i) {
		/* Nullify any elements which are NOT being used */
		if(strcmp(cmd[i],"") == 0) {
			free(copy[i]);
			copy[i] = NULL;
			continue;
		}
		copy[i] = strdup(cmd[i]);
		*param_size = *param_size + 1;
	}
	return (char* const*) copy;
}

int run_command(const char* cmd[], int array_size) {
	int param_size;
	const char* command = cmd[0];
	char * const * params = get_params(cmd, array_size, &param_size);

	if(should_exit(command)) {
		exit(EXIT_SUCCESS);
	}
	if(is_builtin_command(command)) {
		return run_builtin_command(cmd);
	}

	int pid;
	pid = fork();
	if(pid < 0) { /* an error occured */
		print_error("Forking failed - Cannot execute given command.");
		return 0;
	} else if (pid == 0 ) { /* this is the child process */
		char* full_path = get_full_path(command);
		execv(full_path, params);

		/* We only get here if an error occurs in executing
		 * given command */
		print_error("Unknown command\n");
		/* Kill the child process */
		exit(EXIT_FAILURE);

	} else { /* pid > 0 :  i.e. parent process */
		int status;
		/* wait for child process for finish running */
		while(wait(&status) != pid);
	}
	free_pointer_array((void**)params, param_size);
	return 1;
}

int should_exit(const char* cmd) {
	if(strcmp(cmd,"exit") == 0) {
		return 1;
	} else {
		return 0;
	}
}

/**
 * Runs the builtin command.
 * Return 1 on success and 0 on failure.
 */
int run_builtin_command(const char* cmd[]) {
	int cmd_type = get_command_type(cmd[0]);
	switch (cmd_type) {
		case cd: {
			run_change_directory(cmd);
			break;
		}
		case path: {
			run_path_cmd(cmd);
			break;
		}
		case list_history: {
			break;
		}
		case execute_history: {
			break;
		}
		default:
			break;
	}
	return 0;
}

/**
 * Runs the builtin list history command
 */
int run_list_history(void) {
	/* To be implemented*/
	return -1;
}

/**
 * Runs the execute history command.
 * Command is of the form !n where n is an number.
 * At the moment, N supports at most 100 commands.
 */
int run_execute_history(const char* cmd) {
	return -1;
}

/*Runs the PATH builtin command
 *
 * path + [NEW_PATH] : this adds a new path to path search list
 * path - [NEW_PATH] : this removes the given path from the search list
 * path : this displays all currently stored paths separated by a ':'
 *
 */
int run_path_cmd(const char* cmd[]) {
	char key = *cmd[1];
	switch (key) {
		case '+':{ /* add a new path to the list */
			const char* new_path = cmd[2];
			if(new_path == NULL || strcmp(new_path,"") == 0) {
				print_error("Please provide a non-empty path");
				return 0;
			}
			add_string_to_path_list(new_path, &PATH);
			printf("Added new path: %s\n", new_path);
			break;
		}
		case '-': { /* remove path from the list */
			const char* new_path = cmd[2];
			if(new_path == NULL || strcmp(new_path,"") == 0) {
				print_error("Please provide a non-empty path");
				return 0;
			}
			remove_string_from_path_list(new_path, &PATH);
			break;
		}
		default: { /* print all paths */
			if(PATH.size == 0) {
				printf("\nPath list is currently empty\n");
				return 1;
			}
			struct String* curr = PATH.paths;
			for(; curr != NULL; curr = curr->next) {
				printf("%s",curr->data);
				if(curr->next != NULL) {
					printf(":");
				}
			}
			break;
		}
	}
	return 1;
}

/* Runs the change directory command */
int run_change_directory(const char* cmd[]) {
	int ret;
	if(strcmp(cmd[1], "") == 0) { /* No params given*/
		/*
		 * No params given. Return to home directory.
		 */
		uid_t uid = getuid();
		struct passwd* user_info_ptr = getpwuid(uid);
		char* user_home = user_info_ptr->pw_dir;
		if(user_home != NULL) {
			ret = chdir(user_home);
		} else {
			/*
			 * Go to home folder, if user's home dir is unknown.
			 */
			ret = chdir("/home");
		}
	} else {
		ret = chdir(cmd[1]);
	}

	if(ret == -1) { /* an error occured */
		print_error("Changed directory command failed");
		perror(NULL);
		return 0;
	} else {
		return 1;
	}
}

/*
 * Gets the full path represented by the given cmd.
 * The full path is obtained by searching through the
 * global PATH class variable for the first suitable path.
 * Caller is responsible for freeing returned char*
 */
char* get_full_path(const char* cmd) {
	/*To be completed */
	if(is_absolute_path(cmd)) {
		return (char *) cmd;
	} else {
		/* passing null auto-allocates the char buffer */
		char* curr_path = getcwd(NULL, 0);
		if(curr_path == NULL) {
			/* An error occured */
			perror("Failed to get current directory");
			return NULL;
		}
		char* full_path = join_path(curr_path, cmd);
		free(curr_path);
		/* Search the current directory first */
		if(exists_file(full_path) && can_execute_file(full_path)) {
			return full_path;
		}

		/* Otherwise Test all paths in PATH until we find a
		 * suitable Path */
		if(PATH.size > 0) {
			free(full_path);
			struct String* curr = PATH.paths;
			for(; curr != NULL; curr = curr->next) {
				curr_path = curr->data;
				full_path = join_path(curr_path, cmd);
				if(exists_file(full_path) &&
				   can_execute_file(full_path)) {
					return full_path;
				}
				free(full_path);
			}
		}
		/* otherwise we return a non-existent file path based
		 * on the current working directory */
		return full_path;
	}
}

void init(void) {
	initialize_path_list();
}

void initialize_path_list(void) {
	PATH.size = 0;
	PATH.paths = NULL;
}


/**
 * Determines if the given paths has  "/" at the beginning
 * ignoring any white space that preceeds it.
 * i.e. "    /abs" is also an absolute path.
 */
int is_absolute_path(const char* cmd) {
	int i = 0;
	for(; cmd[i] != '\0'; ++i) {
		/*skip over initial whitespace*/
		if(isspace(cmd[i])){
			continue;
		}
		if(cmd[i] == '/'){
			return 1;
		} else {
			return 0;
		}
	}
	return 0;
}

void add_string_to_path_list(const char* string, struct StringList* arr) {
	int size = arr->size;
	int new_size = size + 1;

	/* make new string strcuture with data*/
	struct String* new_string = calloc(1, sizeof(struct String));
	check_allocated_mem("add_string_to_path_list", new_string);

	int string_size = strlen(string);
	char* copy =  calloc(string_size + 1, sizeof(char));
	check_allocated_mem("add_string_to_path_list", new_string);

	/* use string_size + 1 so that copy is null - terminated */
	strncpy(copy, string, string_size + 1);
	new_string->data = copy;
	new_string->size = string_size + 1;

	/* insert new string into list */
	if (size == 0) {
		new_string->next = NULL;
		arr->paths = new_string;

	} else {
		new_string->next = arr->paths;
		arr->paths = new_string;
	}
	arr->size = new_size;
}

/**
 * Removes the given string from the specified Path list
 */
void remove_string_from_path_list(const char* string, struct StringList* list) {
	int deleted = 0;
	struct String* curr = list->paths;
	struct String* first = curr;
	struct String* prev = curr;

	if(list->size == 0) {
		print_error("Cannot remove from PATH - Item not found");
		return;
	}
	/* Handle special case when item to be removed is at the head
	 * of the list  */
	if(strcmp(first->data, string) == 0) {
		struct String* to_delete = first;
		list->paths = first->next;
		list->size--;
		/* free memory */
		free(to_delete->data);
		free(to_delete);
		return;
	}


	prev = first;
	for(; curr != NULL; curr = curr->next) {
		if(strcmp(curr->data, string) == 0 ) {/* found node to remove*/
			prev->next = curr->next;
			list->size--;
			free(curr->data);
			free(curr);
			deleted = 1;
			break;
		}
		/*keep track of previous node */
		prev = curr;
	}
	if(!deleted) {
		print_error("Cannot remove from PATH - Item not found");
	} else {
		printf("Removed %s from PATH", string);
	}
}


/*Returns the type of the  given command with NO arguments.
 * or -1 on error
 */
int get_command_type(const char* cmd) {
	if(strcmp(cmd, "cd") == 0) {
		return cd;
	} else  if(strcmp(cmd, "path") == 0) {
		return path;
	}  else if(strcmp(cmd, "history") == 0) {
		return list_history;
	} else {
		/*check if it's the execute history cmd !n where n is
		 * a number  or not*/
		if(cmd[0] == '!') {
			int i = 1;
			for(; cmd[i] != '\0'; ++i) {
				if(!isdigit(cmd[i])) {
					return -1;
				}
			}
		}
		return execute_history;
	}
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
 * Returns the index of the first eligible path for which the given
 * command is found.
 * Otherwise returns -1 to indicate cmd was not found in any path
 */
int get_first_path_index(const char* cmd, const char* paths[], int array_size) {
	int i = 0;
	for(; i < array_size; ++i) {
		char* full_path = join_path(paths[i], cmd);
		if (exists_file(full_path) && can_execute_file(full_path)) {
			return i;
		}
		free(full_path);
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

/*
 * Frees a dynamically allocated array of pointers.
 * That is, the pointers MUST point to a region of dynamically allocated
 * memory, in addition to the array of pointers itself being dynamically
 * allocated.
 */
void free_pointer_array(void** array, int array_size) {
	int i = 0;
	for(; i < array_size; ++i) {
		if(array[i] != NULL) {
			free(array[i]);
		}
	}
	free(array);
}

/***** TESTS *******/
void test_all(void) {
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

	assert(strcmp(PATH.paths->data, "three") == 0);
	assert(PATH.paths->size == 6);
	assert(strcmp(PATH.paths->next->data, "two") == 0);
	assert(PATH.paths->next->size == 4);
	assert(strcmp(PATH.paths->next->next->data, "one") == 0);
	assert(PATH.paths->next->next->size == 4);
	assert(PATH.size == 3);
}

void test_remove_string_from_path_list(void) {
	init();
	add_string_to_path_list("one", &PATH);
	add_string_to_path_list("two", &PATH);
	add_string_to_path_list("three", &PATH);

	remove_string_from_path_list("two", &PATH);

	assert(strcmp(PATH.paths->data, "three") == 0);
	assert(PATH.paths->size == 6);
	assert(strcmp(PATH.paths->next->data, "one") == 0);
	assert(PATH.paths->next->size == 4);
	assert(PATH.size == 2);
}

void test_is_absolute_path(void) {
	char test[] = "    /abs/test/yes";
	char test2[] = "    no/relative";
	assert(is_absolute_path(test) == 1);
	assert(is_absolute_path(test2) == 0);
}

void test_get_full_path(void) {
	char dir[] = "/tmp";
	char path1[] = "ls";
	char expected1[] = "/tmp/ls";

	char path2[] = "/abs/ls";
	char expected2[] = "/abs/ls";
	chdir(dir);
	assert(strcmp(get_full_path(path1),expected1) == 0);
	assert(strcmp(get_full_path(path2), expected2) == 0);
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
	free_pointer_array((void**)test, size);
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

	assert(strcmp(PATH.paths->data, "THREE") == 0);
	assert(strcmp(PATH.paths->next->data, "TWO") == 0);
	assert(strcmp(PATH.paths->next->next->data, "ONE") == 0);
	run_path_cmd(test4);
	printf("\n\n%s\n", PATH.paths->next->data);
	assert(strcmp(PATH.paths->data, "THREE") == 0);
	assert(strcmp(PATH.paths->next->data, "ONE") == 0);
}
