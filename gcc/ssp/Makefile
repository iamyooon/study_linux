CC=gcc
CFLAGS=-fstack-protector-strong -Wall -g
SAMPLE=ssp

all:  $(SAMPLE)

$(SAMPLE):
	$(CC) $(CFLAGS) ssp.c -o $@

clean:
	rm $(SAMPLE)
