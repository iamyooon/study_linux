#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/types.h>
#include <unistd.h>

pthread_t threads[5];

void *thread_main(void *);

int main(void)
{
	unsigned int i = 0;
	int rc;
	void *status;

	while(1){
		rc = pthread_create(&threads[0], NULL, &thread_main, (void *)0);
		if (rc != 0) {
			printf("ERROR; return code from pthread_create() is %d\n", rc);
			return -1;
		}

		rc = pthread_join(threads[0], &status);
		if (rc != 0) {
			printf("ERROR; return code from pthread_join() is %d\n", rc);
			return -1;
		}
		printf("Completed join with thread %d status= %p\n", i, status);
		i++;
	}
	return 0;
}

void *thread_main(void *arg)
{
	int *ptr;
	ptr = malloc(4096);
	memset(ptr, 5, 1024);
	usleep(1000*10);
	return NULL;
}
