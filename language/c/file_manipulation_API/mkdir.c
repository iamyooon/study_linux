#include <sys/stat.h>
#include <sys/types.h>
#include <errno.h>
#include <stdio.h>

int main()
{
	int result;

	result = mkdir("mkdir_test", 0776);
	if ((result == -1) && (errno != EEXIST))
	{
		puts("error");
		return -1;
	}
	puts("sucess");

	return 0;
}
