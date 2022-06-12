CC ?= gcc

all: likely unlikely

likely:
	$(CC) $(CFLAGS) likely_unlikely.c -o likely -O2 -Dopt=likely

unlikely:
	$(CC) $(CFLAGS) likely_unlikely.c -o unlikely -O2 -Dopt=unlikely

clean:
	rm -rf ./likely
	rm -rf ./unlikely
