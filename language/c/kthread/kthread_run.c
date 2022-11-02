#include <stdio.h>
#include <kthread.h>

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
	kthread_run(tid1, NULL, (void *)thread_fn, NULL);

	while(1){
		printf("%s\n", __func__);
		sleep(1);
	}
	return 0;
}
