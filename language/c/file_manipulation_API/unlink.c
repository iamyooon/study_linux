#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>

int main()
{
	int fd;
	int link_result;
	int unlink_result;


	mode_t mode = S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH;
	char *filename = "./link_org.txt";

	fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, mode);

	link_result = link("link_org.txt", "link.txt");

	if (link_result == -1)
	{
		printf("error\n");
		return link_result;
	}

	unlink_result = unlink("link.txt");

	if (unlink_result == -1)
	{
		printf("error\n");
		return link_result;
	}
	printf("success\n");

	return unlink_result;
}
