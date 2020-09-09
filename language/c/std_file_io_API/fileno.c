#include <stdio.h>
#include <string.h>
#include <unistd.h>

int main()
{
	FILE *fp;
	int fd;

	char str[100] = "fileno test string\n";

	fp = fopen("fileno.txt", "w");
	fd = fileno(fp);

	if (fp == NULL)
		return -1;
	
      	write(fd, str, strlen(str));

	fclose(fp);
}
