#include <stdio.h>
#include <string.h>
#include <pthread.h>

pthread_t threads[5];
int done[5];

void *thread_main(void *);

int main(void)
{
	int i;
	int rc;
	// int status; pthread_join() need to get void pointer's addr
	void *status;

	for (i = 0; i < 5; i++)
	{
		done[i] = 0;
		pthread_create(&threads[i], NULL, &thread_main, (void *)&i);
		printf("thread[%d] create, %ld\n", i, threads[i]);
	}

	for (i = 4; i >= 0; i--)
	{
		done[i] = 1;
		rc = pthread_join(threads[i], &status);
		printf("%p\n", status);
		if (rc == 0)
		{
			printf("Completed join with thread %d status= %p\n",i, status);
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

	printf("thread: %d\n", *((int *)arg));
}

