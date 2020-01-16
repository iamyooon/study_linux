#include <stdio.h>
#include <string.h>

int main()
{
	FILE *fp;
	char input[100] = "fwrite test string\n";

	fp = fopen("fwrite.txt", "w");

	if (fp == NULL)
		return -1;
	
	fwrite(input, strlen(input), 1, fp);

	fclose(fp);
}
