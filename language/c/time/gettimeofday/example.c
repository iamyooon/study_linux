#include <stdio.h>
#include <time.h> // nanosleep
#include <sys/time.h> // gettimeofday()

static struct timeval start, end;

static void show_perf(void)
{
	float usec;
	float msec;

	usec = (end.tv_sec - start.tv_sec) * 1000000 + (end.tv_usec - start.tv_usec);
	msec = usec / 1000;

	printf("elapsed time(usec)=%.5f,msec=%.5f\n", usec, msec);
}


int main(int argc, char** argv)
{

	gettimeofday(&start, NULL);
	nanosleep((const struct timespec[]){{1, 0}}, NULL);
	gettimeofday(&end, NULL);

	show_perf();

	gettimeofday(&start, NULL);
	nanosleep((const struct timespec[]){{0, 5 * 1000 * 1000L}}, NULL);
	gettimeofday(&end, NULL);

	show_perf();

	return 0;
}
