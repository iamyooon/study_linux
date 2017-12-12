CC ?= gcc

all:
	$(CC) ./pthread_create.c -lpthread -o ./pthread_create
	$(CC) ./pthread_join.c -lpthread -o ./pthread_join
	$(CC) ./pthread_setname.c -lpthread -o ./pthread_setname
	$(CC) ./pthread_mutex.c -lpthread -o ./pthread_mutex
clean:
	rm -rf pthread_create pthread_join pthread_setname pthread_mutex
