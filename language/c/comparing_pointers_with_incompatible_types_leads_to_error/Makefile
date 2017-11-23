#CC ?= arm-linux-gnueabi-gcc
CC ?= gcc
CFLAGS ?= -Werror

all: test_example

test_example:
	$(CC) $(CFLAGS) comparing_pointers_with_incompatible_types.c -o example
clean:
	rm -rf ./example
