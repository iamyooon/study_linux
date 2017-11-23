#CC ?= arm-linux-gnueabi-gcc
CC ?= gcc

all: test_example

test_example:
	$(CC) when_thread_is_recursively_create_and_destroy_with_allocated_mem.c -lpthread -o example 
clean:
	rm -rf ./example
