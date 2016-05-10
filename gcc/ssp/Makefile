CC=gcc
CFLAGS=-fstack-protector -Wall
SAMPLE=ssp

all:  $(SAMPLE)

$(SAMPLE):
	$(CC) $(CFLAGS) ssp.c -o $@

clean:
	rm $(SAMPLE)
