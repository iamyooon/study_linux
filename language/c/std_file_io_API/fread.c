#include <stdio.h>
#include <string.h>

int main()
{
	FILE *fp;
	char input[100] = "fread OK!\n";
	char output[100];

	// fwrite
	fp = fopen("fread.txt", "w");
	if (fp == NULL)
		return -1;
	
	fwrite(input, strlen(input), 1, fp);
	fclose(fp);

	// fread
	fp = fopen("fread.txt", "r");
	if (fp ==NULL)
		return -1;

	fread(output, sizeof(output), 1, fp);
	printf("fread string: %s", output);

	fclose(fp);

	return 0;
}
