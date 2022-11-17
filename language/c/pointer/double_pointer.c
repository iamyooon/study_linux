#include <stdio.h>

void foo(int **ptr)
{
	if(ptr)
		printf("[%s] ptr's addr = %x %x\n", __func__, (int *)ptr, (int *)&ptr);
}

int main(int argc, char** argv)
{
	int *ptr=NULL;
	printf("[%s] ptr's addr = %x %x\n", __func__, (unsigned int)ptr, (unsigned int)&ptr);
	foo(&ptr);
}
