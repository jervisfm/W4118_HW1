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
#endif /* SHELL_H_ */
