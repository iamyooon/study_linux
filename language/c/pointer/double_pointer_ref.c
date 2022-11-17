#include <stdio.h>

int main(int argc, char** argv)
{
	char* arg[]={"0","165","1"};
	printf("%p %p %p %p %s\n", arg, arg+1, arg+2, arg+3, *(arg+3));
	return 0;
}
