#include <stdio.h>

int userdata = 987654321;
int main(int argc, char** argv)
{
	void *pdata = &userdata;

	int *arg = 0;
	printf("*arg=*(0)->segfault,arg=%x\n", arg);

	arg = (int *)pdata;
	printf("*arg=%x,(int *)pdata=%x,arg=%x\n", *arg, (int *)pdata,arg);
	return 0;
}
