#CC ?= arm-linux-gnueabi-gcc
CC ?= gcc

all: test_example

test_example:
	$(CC) $(CFLAGS) getpwnam.c -o getpwnam
	$(CC) $(CFLAGS) getgrnam.c -o getgrnam
clean:
	rm -rf ./example
