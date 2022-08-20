#include <stdio.h>
#include <unistd.h>	//getopt()

int main(int argc, char** argv)
{
	int *dest;
	char paylen[24]={0,};

	dest = (int *)paylen;

	printf("%d\n",*dest);
	*dest = 1024*1024;
	printf("%d\n",*dest);
}
