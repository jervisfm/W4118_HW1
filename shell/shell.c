
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <assert.h>
#include "shell.h"


int test (int argc, char** argv)
{
	char* c = read_line();

	printf("\n\n%s\n", c);
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
 * Caller responsible for feeing returned line string
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
 */
void parse_line(const char* line, char* parsed[])
{

}

enum ParseMode {
	reading, skipping
};
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
				break;
			}
			case skipping:
			{
				if(size > max) {
					max = size;
				}
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

/***** TESTS *******/
void test_all(void) {
	test_get_maximum_string();
}

int test_print(void) {
	printf(TEXT);
	return 0;
}
void test_get_maximum_string(void) {
//	char test[] = " This is a test string";
//	char test2[]= "This is a VeryLongTestString";

	assert(1 == 1);
}

