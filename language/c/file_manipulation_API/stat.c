#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdio.h>
#include <time.h>

int main()
{
	struct stat sb;
	int result;

	result = stat("./", &sb);
	if (result == -1) {
		printf("error\n");
	}
	printf("I-node number: %ld\n", (long int)sb.st_ino);
	printf("Ownership: UID=%ld, GID=%ld\n", (long int)sb.st_uid, (long int) sb.st_gid);
	printf("File size: %lld bytes\n", (long long int) sb.st_size);
	printf("Last file access: %s", ctime(&sb.st_atime));

	return 0;
}
