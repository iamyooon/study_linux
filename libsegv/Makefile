CC := arm-linux-gnueabi-gcc
CFLAGS := -g3 -O0 -funwind-tables -rdynamic -fno-omit-frame-pointer

all: arm x86
	mkdir -p output
	mv *.so *.o  sample_segv* ./output
x86:
	gcc $(CFLAGS) -c -fPIC segv.c -o libsegv_x86.o
	gcc libsegv_x86.o -shared -o libsegv_x86.so
	gcc $(CFLAGS) sample.c -o sample_segv_x86
arm:
	$(CC) $(CFLAGS) -c -fPIC segv.c -o libsegv_arm.o
	$(CC) libsegv_arm.o -shared -o libsegv_arm.so
	$(CC) $(CFLAGS) sample.c -o sample_segv_arm
clean:
	rm -rf output
