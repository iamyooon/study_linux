#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include <sys/types.h>
#include <unistd.h>

pthread_t threads[5];
int done[5];

void *thread_main(void *);
pthread_mutex_t pet_flag_sync;

int main(void)
{
	int i;
	int rc;
	int status;

	printf("pid=%d\n", getpid());

	pthread_mutex_init(&pet_flag_sync, NULL);

	pthread_mutex_lock(&pet_flag_sync);
	for (i = 0; i < 5; i++)
	{
		done[i] = 0;
		pthread_create(&threads[i], NULL, &thread_main, (void *)&i);
		printf("thread[%d] create, %lu\n", i, threads[i]);
	}
	pthread_mutex_unlock(&pet_flag_sync);

	sleep(5);

	for (i = 4; i >= 0; i--)
	{
		done[i] = 1;
		rc = pthread_join(threads[i], (void **)&status);
		printf("%d\n", status);
		if (rc == 0)
		{
			printf("Completed join with thread %d status= %d\n",i, status);
		}
		else
		{
			printf("ERROR; return code from pthread_join() is %d, thread %d\n", rc, i);
			return -1;
		}
	}

	return 0;
}

void *thread_main(void *arg)
{
	int i;
	double result=0.0;

	pthread_mutex_lock(&pet_flag_sync);
	printf("thread: %d, %d\n", *((int *)arg), getpid());
	pthread_mutex_unlock(&pet_flag_sync);
}

