#include <fcntl.h>
#include <unistd.h>

int main()
{
	int fd;
	mode_t mode = S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH;
	char *filename = "./close.txt";

	fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, mode);

	close(fd);
}
