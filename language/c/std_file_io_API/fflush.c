#include <stdio.h>

int main()
{
	int i;

	// 루프를 돈 후에 1234567890를 출력해줌
	printf("12345");
	for (i = 0; i >= 0; i++);
	printf("67890\n");

	// 12345를 출력한 후 루프를 돌고 67890를 출력해줌
	printf("12345");
	fflush(stdout);
	for (i = 0; i >= 0; i++);
	printf("67890\n");

	return 0;
}
