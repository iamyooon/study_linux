#CC = arm-linux-gnueabi-gcc
CC ?= gcc
CFLAGS = -fsanitize=address -fno-omit-frame-pointer -g -funwind-tables

all: example

example:
	$(CC) $(CFLAGS) example.c -o example
clean:
	rm -rf ./example
