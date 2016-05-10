CC=gcc
CFLAGS=-shared -fpic
LDFLAGS=-ldl
LIBHOOK=libhooker.so
SAMPLE=getenv

all: $(LIBHOOK) $(SAMPLE)

$(LIBHOOK):
	#occur error
	#$(CC) $(CFLAGS) $(LDFLAGS) hooker.c -o $@
	$(CC) $(CFLAGS) hooker.c -o $@ $(LDFLAGS)

$(SAMPLE):
	$(CC) getenv.c -o $@

clean:
	rm $(LIBHOOK) $(SAMPLE)
