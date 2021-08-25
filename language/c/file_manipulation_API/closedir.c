#include <dirent.h>
#include <unistd.h>
#include <stdio.h>

int main()
{
	DIR *dir;
	dir = opendir("./");
	if (dir == NULL) {
		puts("error");
		return 1;
	}
	puts("open directory");

	closedir(dir);
	if (dir != 0) {
		puts("error");
		return 1;
	}
	puts("close directory");

	return 0;
}
