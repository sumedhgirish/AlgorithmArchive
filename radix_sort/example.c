#define RADIX_IMPLEMENTATION
#include "radix_sort.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char* argv[])
{
	if(argc != 2)
	{
		printf("Usage: %s <characters>\n", argv[0]);
		exit(1);
	}
	char* numbers = strdup(argv[1]);
	printf("Initial: %s\n", numbers);
	int status = RadixSort(numbers, strlen(numbers));
	printf("Final  : %s\n", numbers);
	return 0;
}
