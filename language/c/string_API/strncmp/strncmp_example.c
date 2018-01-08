#include <stdio.h>
#include <string.h>

int main(int argc, char **argv)
{
	int result = -1;
	char str1[32]="iello world";
	char str2[32]={'\0',};
	//char str2[32]={0,};

	result = strncmp(str1, str2, sizeof(str2));
	printf("result=%d %d %d\n", result, 'i','0');

	return 0;
}
