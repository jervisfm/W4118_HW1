/*
 * shell.h
 *
 *  Created on: Sep 9, 2012
 */

#ifndef SHELL_H_
#define SHELL_H_

#define TEXT "test text"

struct String {
	char* data; /*string data*/
	int size; /*includes null character */
	struct String* next; /*Next list item*/
};

struct StringList {
	struct String* head; /*Head of dynamic list*/
	int size; /*current number of elements*/
};

/* Stores the Search paths*/
struct StringList PATH;
struct StringList HISTORY;


/**
 * The maximum number of arguments that can be input
 * on the shell, including the actual command.
 * Thus, if set to 11, the actual maximum arguments possible is
 * 10.
 */
#define MAX_ARGUMENTS 11
#define MAX_HISTORY_ITEMS 100

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

/*Initialize relevant global variables & structures*/
void init(void);

/* Initialize array structure that stores list of exec paths*/
void initialize_path_list(void);

void initialize_history_list(void);

/*Adds a new string to path array*/
void add_string_to_path_list(const char* string, struct StringList* arr);

void remove_string_from_path_list(const char* string, struct StringList* list);

void add_string_to_history_list(const char* string, struct StringList* list);

void delete_head_from_list(struct StringList* list);

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

int run_builtin_command(const char* cmd[]);

char* get_full_path(const char* cmd);

int run_list_history(void);

int run_path_cmd(const char* cmd[]);

int is_absolute_path(const char* cmd);

char* const* get_params(const char* cmd[], int array_size, int* param_size);

int run_shell(void);

void free_pointer_array(void** array, int array_size);

int should_exit(const char* cmd);

int run_change_directory(const char* cmd[]);

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

#endif /* SHELL_H_ */
