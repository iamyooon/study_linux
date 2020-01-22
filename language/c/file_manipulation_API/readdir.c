#include <dirent.h>
#include <unistd.h>
#include <stdio.h>

int main()
{
	DIR *dir;
	struct dirent *dir_entry;
	int i = 0;

	dir = opendir("./");
	if (dir == NULL) {
		puts("error");
		return 1;
	}
	else
	{
		while (dir_entry = readdir(dir))
		{
			i++;
			printf("file list %d: %s\n", i, dir_entry->d_name);
		}
		closedir(dir);
	}

}
