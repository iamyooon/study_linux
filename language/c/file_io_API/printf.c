#include <stdio.h>
#include <stdlib.h>

int main()
{
	printf("문자열[%s]\n", "Hello World");
	printf("문자 1개[%c]\n", 'H');
	printf("부호있는정수[%d]\n", -3);
	printf("실수[%f]\n", 2.1234);
	printf("실수 소수 2자리만[%0.2f]\n", 2.1234);
	printf("16진수 대문자[%X]\n", 'H');
	printf("16진수 소문자[%x]\n", 'i');
	printf("16진수 2자리만[%02X]\n", 255);
	return 0;
}
