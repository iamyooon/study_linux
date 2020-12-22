#include <stdio.h>
#include <pthread.h>	// pthread_t, pthread_create()
#include <unistd.h> 	// sleep()

void thread_fn(void *data)
{
	int i = 0;

	printf("in thread_fn\n");
	while(1) {
		i++;
	}
}

int main(int argc, char** argv)
{

	pthread_t tid1, tid2, tid3, tid4;
	pthread_create(&tid1, NULL, (void *)thread_fn, NULL);
	pthread_create(&tid2, NULL, (void *)thread_fn, NULL);
	pthread_create(&tid3, NULL, (void *)thread_fn, NULL);
	pthread_create(&tid4, NULL, (void *)thread_fn, NULL);

	while(1) {
		sleep(1);
	}

	return 0;
}
