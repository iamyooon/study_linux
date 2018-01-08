#include <stdio.h>

int main(int argc, char** argv)
{
	unsigned int cnt = 0;
	unsigned long long cnt2=0;

	do{
		cnt2++;
	}while(--cnt);

	printf("loop is iterated over %llu\n", cnt2);

	return 0;
}
