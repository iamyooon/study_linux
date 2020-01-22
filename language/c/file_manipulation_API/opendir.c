#include <dirent.h>
#include <stdio.h>

int main()
{
	DIR *dir;
	dir = opendir("./");
	if (dir == NULL) {
		puts("error");
		return 1;
	}
	puts("success");

	return 0;
}
