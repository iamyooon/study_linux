#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>

int main()
{
	int fd;
	int rename_result;

	mode_t mode = S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH;
	char *filename = "./rename_org.txt";

	fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, mode);

	rename_result = rename(filename, "rename.txt");
	if (rename_result == -1)
	{
		printf("error");
	}
	printf("success");

	return rename_result;
}
