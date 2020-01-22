#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <stdio.h>

int main()
{
	struct stat sb, sb_new;
	int stat_st;
	int chown_st;

	// read origin owner
	stat_st = stat("./chown.c", &sb);
	printf("UID=%ld, GID=%ld\n", (long int)sb.st_uid, (long int) sb.st_gid);

	// change owner
	chown_st = chown("./chown.c", 1001, 1001);
	if (chown_st == -1) {
		printf("error\n");
		return chown_st;
	}
	stat_st = stat("./chown.c", &sb_new);
	printf("UID=%ld, GID=%ld\n", (long int)sb_new.st_uid, (long int) sb_new.st_gid);

	// change owner to origin
	chown_st = chown("./chown.c", sb.st_uid, sb.st_gid);
	if (chown_st == -1) {
		printf("error\n");
		return chown_st;
	}
	stat_st = stat("./chown.c", &sb);
	printf("UID=%ld, GID=%ld\n", (long int)sb.st_uid, (long int) sb.st_gid);


	return 0;
}
