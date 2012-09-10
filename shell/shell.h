/*
 * shell.h
 *
 *  Created on: Sep 9, 2012
 */

#ifndef SHELL_H_
#define SHELL_H_

#define TEXT "test text"
/**
 * The maximum number of arguments that can be input
 * on the shell, including the actual command.
 * Thus, if set to 11, the actual maximum arguments possible is
 * 10.
 */
#define MAXIMUM_ARGUMENTS 11

/**
 * Reading - we count this character as being part of the string
 * Skipping - we skip over the "whitespace" character and reset count.
 */
enum ParseMode {
	reading, skipping
};

/* Number of builtin commands supported by this shell*/
#define BUILTIN_COMMANDS_SIZE 3
enum CommandType {
	cd, path, list_history, execute_history
};
char* BUILTIN_COMMANDS[] = {"cd", "path", "history"};


int test_print(void);

/*
 * Reads from standard input.
 * Caller responsible for freeing returned line string
 */
char* read_line();

/**
 * Prints the given error string to stdout.
 * The formated used is 'error: %s'
 */
void print_error(const char* err);

/**
 * Print the shell prompt
 */
void print_prompt();

/**
 * This parses the given line into separate string.
 * The delimeter used by default is whitespace.
 */
void parse_line(const char* line, char* parsed[], const int size);

/**
 * Finds the maximum string in the given line.
 */
int get_maximum_string(const char* line);

void initialize_string_array(char* array[], int buffer_size, int array_size);

int is_builtin_command(const char* line);

int run_command(const char* cmd[], int array_size);

int exists_file(const char* file_path);

int can_execute_file(const char* file_path);

void check_allocated_mem(const char* function, void * input);

char* join_path(const char* dir, const char* path);

int get_first_path_index(const char* cmd, const char* paths[], int array_size);

int get_command_type(const char* cmd);

int run_builtin_command(const char* cmd);

char* get_full_path(const char* cmd);

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

#endif /* SHELL_H_ */
