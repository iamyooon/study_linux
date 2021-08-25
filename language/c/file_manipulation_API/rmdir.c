#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>

int main()
{
	int result;

	result = mkdir("rmdir_test", 0776);
	if (result == -1)
	{
		puts("error");
		return result;
	}
	puts("make directory");

	result = rmdir("rmdir_test");
	if (result == -1)
	{
		puts("error");
		return result;
	}
	puts("success");

	return result;
}
