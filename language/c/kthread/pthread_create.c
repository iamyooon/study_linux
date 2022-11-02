#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

void thread_fn2(void *data)
{
	while(1){
		sleep(1);
		printf("%s\n", __func__);
	}
}

void thread_fn(void *data)
{
	while(1){
		sleep(1);
		printf("%s\n", __func__);
	}
}

int main(int argc, char** argv)
{
	pthread_t tid1, tid2;
	pthread_create(&tid1, NULL, (void *)thread_fn, NULL);
	pthread_create(&tid2, NULL, (void *)thread_fn2, NULL);

	while(1){
		printf("%s\n", __func__);
		sleep(1);
	}
	return 0;
}
