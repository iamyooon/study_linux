#CC ?= arm-linux-gnueabi-gcc
CC ?= gcc

all: test_example

test_example:
	$(CC) $(CFLAGS) example.c -o example
clean:
	rm -rf ./example
