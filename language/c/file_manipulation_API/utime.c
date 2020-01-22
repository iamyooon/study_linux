#include <sys/types.h>
#include <utime.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

int main()
{
	struct stat sb;
	int result;

	stat("./utime.c", &sb);
	printf("Last file access: %s", ctime(&sb.st_atime));

	// 현재 시간으로 변경
	result = utime("./utime.c", NULL);

	if (result) {
		printf("error\n");
		return result;
	}

	stat("./utime.c", &sb);
	printf("Last file access: %s", ctime(&sb.st_atime));

	return result;
}
