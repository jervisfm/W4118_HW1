/*
 * shell.h
 *
 *  Created on: Sep 9, 2012
 */

#ifndef SHELL_H_
#define SHELL_H_

#define TEXT "test text"
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
#endif /* SHELL_H_ */
