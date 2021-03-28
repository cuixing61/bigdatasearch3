#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <windows.h>

#include "trie.h"


int main(int argc, char **argv)
{
	tree test;
	init(&test);
	insertFromFile(&test, "C:\\cuixing\\127w.txt");
	queryInFile(&test, "C:\\cuixing\\content.txt");
	printToFile(&test, "C:\\cuixing\\result_unsorted.txt");

	printf("all done\n");

	system("pause");
	return 0;
}
