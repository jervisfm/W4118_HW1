
#include <stdlib.h>
#include <stdio.h>
#include "shell.h"

int test (int argc, char** argv)
{
	char* c = read_line();

	printf("\n\n%s\n", c);
	return 0;
}

int main(int argc, char **argv)
{
	return test(argc, argv);
}

/*
 * Reads line from stdin
 * Caller responsible for feeing returned line string
 */
char* read_line()
{
	char c = '\0';
	char* line = (char *) calloc(1, sizeof(char));

	/*memory allocation failed*/
	if(line == NULL) {
		perror("error: readline memory allocation failed");
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
		line[i] = c;
		line[i+1] = '\0';
		++i;
	}
	return line;
}

void print_error(const char* err) {
	printf("error: %s", err);
}

int test_print(void) {
	printf(TEXT);
	return 0;
}
