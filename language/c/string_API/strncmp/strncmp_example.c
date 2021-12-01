#include <stdio.h>
#include <string.h>

int main(int argc, char **argv)
{
	int result = -1;
	char str1[32]="iello world";
	char str2[32]={'\0',};
	char *str3;
	char str4[32];
	str3 = "hello";
	//char str2[32]={0,};

	int i=0;
	for (i=0; i<32; i++) {
		str4[i]=str3[i];
	}
	printf("str3=%s\n", str3);
	printf("str4=%s\n", str4);

	//result = strncmp(str1, str2, sizeof(str2));
	//printf("result=%d %d %d\n", result, 'i','0');

	return 0;
}
