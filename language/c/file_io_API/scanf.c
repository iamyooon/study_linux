#include <stdio.h>
#include <stdlib.h>

int main()
{
	int selnum = 0;
	char strcontent[128] = {0,};
	
	printf("숫자를 입력하세요\n");
	scanf("%d", &selnum);
	printf("입력한 숫자는 [%d]\n", selnum);

	printf("문자를 입력하세요\n");
	scanf("%s", strcontent);
	printf("입력한 문자는 [%s]\n", strcontent);

	return 0;
}
