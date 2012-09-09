
#include <stdlib.h>
#include <stdio.h>
#include "shell.h"

int main(int argc, char **argv)
{
	test_print();
	return 0;
}

int test_print(void) {
	printf(TEXT);
	return 0;
}
