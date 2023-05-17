#CROSS_COMPILE=aarch64-linux-gnu-
all:
	${CROSS_COMPILE}gcc -c addition.c
	${CROSS_COMPILE}g++ -o program main.cpp addition.o

clean:
	rm -rf program addition.o
