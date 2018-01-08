#define _GNU_SOURCE
#include <pthread.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <stdlib.h>
#include <sys/prctl.h>

#define NAMELEN 16

#define errExitEN(en, msg) \
	do { errno = en; perror(msg); exit(EXIT_FAILURE); \
	} while (0)

void print_comm_using_prctl(void)
{
	char pr_name[32] = {0,};
	int ret = 0;

	ret = prctl(PR_GET_NAME, pr_name, NULL, NULL, NULL);
	if (ret != 0){
		printf("%s : prctr() failed, ret=%d\n", __func__, ret);
		return ;
	}
	printf("%s : process name is %s\n", __func__, pr_name);
}
static void *threadfunc(void *parm)
{
	sleep(5);          // allow main program to set the thread name
	print_comm_using_prctl();
	return NULL;
}

int main(int argc, char **argv)
{
	pthread_t thread;
	int rc;
	char thread_name[NAMELEN];

	print_comm_using_prctl();

	rc = pthread_create(&thread, NULL, threadfunc, NULL);
	if (rc != 0)
		errExitEN(rc, "pthread_create");

	rc = pthread_getname_np(thread, thread_name, NAMELEN);
	if (rc != 0)
		errExitEN(rc, "pthread_getname_np");

	printf("Created a thread. Default name is: %s\n", thread_name);
	rc = pthread_setname_np(thread, (argc > 1) ? argv[1] : "THREADFOO");
	if (rc != 0)
		errExitEN(rc, "pthread_setname_np");
	sleep(2);

	rc = pthread_getname_np(thread, thread_name,
			(argc > 2) ? atoi(argv[1]) : NAMELEN);
	if (rc != 0)
		errExitEN(rc, "pthread_getname_np");
	printf("The thread name after setting it is %s.\n", thread_name);

	rc = pthread_join(thread, NULL);
	if (rc != 0)
		errExitEN(rc, "pthread_join");

	printf("Done\n");
	exit(EXIT_SUCCESS);
}

