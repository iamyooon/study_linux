#include <stdio.h>
#include <fcntl.h>
#include <string.h>

int main()
{
	FILE *fp;
	int fd;

	char str[100] = "fdopen test string\n";

	fd = open("fdopen.txt", O_RDONLY);
	fp = fopen("fdopen.txt", "w");

	if (fp == NULL)
		return -1;
	
	fwrite(str, strlen(str), 1, fp);

	fclose(fp);
}
	
