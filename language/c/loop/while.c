#include <stdio.h>

int func1(int val)
{
	return val;
}

int main(int argc, char** argv)
{
	int ret;

	printf("before func1(0) \n");
	while((ret = func1(0)) > 0) {
		sleep(1);
		printf("hello\n");
	}
	printf("after func1(0)\n");
	printf("before func1(1)\n");
	while((ret = func1(1)) > 0) {
		sleep(1);
		printf("hello\n");
	}
	printf("after func1(1)n");
	return 0;
}
