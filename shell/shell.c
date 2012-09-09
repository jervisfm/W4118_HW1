
#include <stdlib.h>
#include <stdio.h>
#include "shell.h"

int main(int argc, char **argv)
{
	read_line();
	return 0;
}

char* read_line()
{
	char c = '\0';
	char* line = (char *) calloc(1, sizeof(char));

	/*current index into line*/
	int i = 0;

	/*this includes the null character.
	 * so an empty string has size of 1
	 */
	int string_size = 1;
	printf("\n");
	while (scanf("%c", &c) > 0 && c != '\n') {
		line = realloc(line, ++string_size);
		line[i] = c;
		printf("%c", c);
		++i;
	}
	printf("\n");
	return line;
}

int test_print(void) {
	printf(TEXT);
	return 0;
}
