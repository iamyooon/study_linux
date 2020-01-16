#include <fcntl.h>

int main()
{
	int fd;
	mode_t mode = S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH;
	char *filename = "./creat.txt";
	fd = creat(filename, mode);
}
