#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

pthread_mutex_t sync_mutex;
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;

void *wewake_func_a(void *data)
{
        printf("sub - lock\n");
        pthread_mutex_lock(&sync_mutex);
        printf("sub - sleep 10s\n");
	sleep(10);
        printf("sub - about to exit\n");
	pthread_exit(0);
}

int main(int argc, char** argv)
{
        pthread_t p_thread;
        int status;

        pthread_mutex_init(&sync_mutex, NULL);

        printf("main - start sub\n");
        pthread_create(&p_thread, NULL, wewake_func_a, "wewake_func_a");

        printf("main - sleep 2s for waiting a lock to be locked by sub\n");
        sleep(2);

        printf("main - wait join\n");
        pthread_join(p_thread, (void **)&status);

	if (argc > 1) {
		printf("main - unlock mutex\n");
		pthread_mutex_unlock(&sync_mutex);
	}

        printf("main - wait to aquire mutex\n");
	pthread_mutex_lock(&sync_mutex);

	printf("main - after aquire mutex\n");
        return 0;
}
