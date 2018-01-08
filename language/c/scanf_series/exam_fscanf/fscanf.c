#include <stdio.h>

int main(int argc, char **argv)
{
	int val = -1;
	int ret = -1;
	ret = fscanf(stdin, "%dh", &val);
	printf("ret = %d val = %d\n", ret, val);
	return 0;
}
