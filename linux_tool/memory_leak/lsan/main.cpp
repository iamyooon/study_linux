#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sanitizer/lsan_interface.h>

void kkk() {
	malloc(5);
}

void FooBar() {
	malloc(7);
}

void handler(int signum) {
	__lsan_do_leak_check();
}

int main(int argc, char** argv) {
	int i=5;

	signal(SIGINT, handler);

	if (argc > 1) {
		while(i--) {
			FooBar();
			sleep(1);
			printf("%d\n", i);
		}

		i=3;
		while(i--) {
			kkk();
			sleep(1);
			printf("%d\n", i);
		}
	}else {
		FooBar();
	}
	return 0;
}
