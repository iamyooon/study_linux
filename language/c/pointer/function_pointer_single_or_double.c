#include <stdio.h>

void foo(void)
{
	printf("hello %s\n", __func__);
}

int main(int argc, char** argv)
{
	printf("%p %p\n", foo, &foo);
	return 0;
}
