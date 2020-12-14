#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
int main(void)
{
	int current_uid = getuid();
	int current_gid = getgid();

	printf("My UID is: %d. My GID is: %d\n", current_uid, getgid());
	system("/usr/bin/id");
	if (setuid(0))
	{
		perror("setuid");
		return 1;
	}

	//I am now root!
	printf("My UID is: %d. My GID is: %d\n", getuid(), getgid());
	system("/usr/bin/id");

	//Time to drop back to regular user privileges
	setuid(current_uid);
	setgid(current_gid);
	printf("My UID is: %d. My GID is: %d\n", getuid(), getgid());
	system("/usr/bin/id");

	return 0;
}
