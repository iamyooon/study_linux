/*
 * source from http://www.informit.com/articles/article.aspx?p=23618&seqNum=4
 */
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

int main (int argc, char* argv[])
{
	char* file = argv[1];
	int fd;
	struct flock lock;
	int ret;

	printf ("opening %s\n", file);
	/* Open a file descriptor to the file. */
	fd = open (file, O_WRONLY);
	printf ("locking\n");
	/* Initialize the flock structure. */
	memset (&lock, 0, sizeof(lock));
	lock.l_type = F_WRLCK;
	/* Place a write lock on the file. */
	ret = fcntl (fd, F_SETLKW, &lock);
	if (ret == 0)
		printf ("locked; hit Enter to unlock... ");
	else
		perror("!!");
	/* Wait for the user to hit Enter. */
	getchar ();

	printf ("unlocking\n");
	/* Release the lock. */
	lock.l_type = F_UNLCK;
	fcntl (fd, F_SETLKW, &lock);

	close (fd);
	return 0;
}
