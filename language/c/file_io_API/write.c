#include <stdio.h>         // printf()
#include <string.h>        // strlen()
#include <fcntl.h>         // O_WRONLY
#include <unistd.h>        // write(), close()

int main()
{
	char *temp = "wewakecorp";
   	int fd;

   	if (0 < (fd = open("./test.txt", O_WRONLY | O_CREAT | O_EXCL, 0644)))
   	{
      		write(fd, temp, strlen(temp));
      		close(fd);
   	}
   	else
	{
		printf("file open failed.\n");
	}
   	return 0;
}
