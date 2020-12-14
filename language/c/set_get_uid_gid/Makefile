#CC ?= arm-linux-gnueabi-gcc
CC ?= gcc

all: test_example

test_example:
	$(CC) $(CFLAGS) main.c -o example
	sudo chown root.root ./example
	sudo chmod +s ./example
clean:
	rm -rf ./example
