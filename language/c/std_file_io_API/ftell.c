#include <stdio.h>
#include <string.h>

int main()
{
	int file_ptr;
	FILE *fp;

	char input[100] = "fwrite test string";

	fp = fopen("ftell.txt", "w");

	if (fp == NULL)
		return -1;
	
	fwrite(input, strlen(input), 1, fp);
	fclose(fp);

	fp = fopen("ftell.txt", "r");

	fseek(fp, 0, SEEK_END);
	file_ptr = ftell(fp);

	printf("%d\n", file_ptr);

	fclose(fp);

	return 0;
}
