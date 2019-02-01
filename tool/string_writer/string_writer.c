#include <systemd/sd-daemon.h>
#include <stdio.h>
#include <unistd.h>
#include <errno.h> // errno
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>


int main(int argc, char** argv)
{
	char *data, *filepath;
	int fd, ret;
        sd_notify(0, "READY=1");

	if (argc == 1) {
		fprintf(stderr, "argc=%d argv=%x\n", argc, argv);
		return -1;
	}

	if (argc == 2)
		data = "hello world";
	else
		data = argv[2];

	filepath = argv[1];

	fprintf(stderr, "Write [%s] to [%s]\n", data, filepath);

	fd = open(filepath, O_WRONLY);
	if (fd < 0) {
		fprintf(stderr, "open() failed, errno=%d\n", errno);
		return -1;
	}

	ret = write(fd, data, 256);
	if (ret < 0) {
		fprintf(stderr, "write() failed, errno=%d\n", errno);
	}

	while(1){
		fprintf(stderr, "Hanging...\n");
		sleep(1);
	}

        return 0;
}
