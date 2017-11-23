/*
 * ref. https://stackoverflow.com/questions/10393844/about-typecheck-in-linux-kernel
 */
#include <stdio.h>

int main(int argc, char** argv)
{
	int a;
	char b;
	//int b;

	(void)(&a == &b);

	printf("Hello world\n");

	return 0;
}
