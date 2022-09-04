#CC ?= arm-linux-gnueabi-gcc
CC ?= gcc

all: build_example

build_example:
	$(CC) $(CFLAGS) src/example.c -o example

clean:
	rm -rf ./example
