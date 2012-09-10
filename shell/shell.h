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
void parse_line(const char* line, char* parsed[]);

/**
 * Finds the maximum string in the given line.
 */
int get_maximum_string(const char* line);


/**
 * Tests
 */
void test_all(void);
void test_get_maximum_string(void);


#endif /* SHELL_H_ */
