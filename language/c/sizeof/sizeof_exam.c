#include <stdio.h>
#include <stdlib.h>

struct exam{
	int a;
	int b;
	char c;
};

int main(int argc, char** argv)
{
	char arr[64] = {0,};
	struct exam ex;
	struct exam *pex;
	int *ptr;

	printf("sizeof(arr[64]) = %ld\n",sizeof(arr));
	printf("sizeof(ex) = %ld\n",sizeof(ex));
	printf("sizeof(pex) = %ld\n",sizeof(pex));
	printf("sizeof(ptr) = %ld\n",sizeof(ptr));

	ptr = (int *)malloc(64);

	printf("after pointing malloced area, sizeof(ptr) = %ld\n",sizeof(ptr));

	return 0;
}
