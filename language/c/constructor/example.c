#include <stdio.h>

int main(int argc, char** argv)
{
	printf("hello world\n");
}

void __attribute__((constructor)) init(void)
{
	printf("hi world\n");
}
