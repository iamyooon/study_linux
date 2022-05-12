#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/stat.h>

int main()
{
	char *string = NULL;
	string = getenv("LD_PRELOAD");
	if(string){
		printf("LD_PRELOAD detected through getenv()\n");
		printf("string = %s\n", string);
	} else
		printf("Environment is clean\n");

	if(open("/etc/ld.so.preload", O_RDONLY) > 0)
		printf("/etc/ld.so.preload detected through open()\n");
	else
		printf("/etc/ld.so.preload is not present\n");

	return 0;
}
