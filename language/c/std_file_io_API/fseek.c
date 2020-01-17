#include <stdio.h>
#include <string.h>

int main()
{
	FILE *fp;
	char str[1024] = "0123456789abcdef";

	fp = fopen("fseek.txt", "w");

	if (fp == NULL)
		return -1;
	
	fwrite(str, strlen(str), 1, fp);
	fclose(fp);

	fp = fopen("fseek.txt", "r");

	printf("%s\n", fgets(str, 1024, fp));

	fseek(fp, 11, SEEK_SET); // 파일의 시작에서 10bytes 앞으로 이동
	printf("%s\n", fgets(str, 1024, fp));

	fseek(fp, 5, SEEK_SET); // 파일의 시작에서 5bytes 앞으로 이동
	fseek(fp, 5, SEEK_CUR);// 현재 위치에서 5bytes 앞으로 이동
	printf("%s\n", fgets(str, 1024, fp));

	fseek(fp, -5, SEEK_END); // 파일의 끝에서 5bytes 뒤로 이동
	printf("%s\n", fgets(str, 1024, fp));

	fclose(fp);

	return 0;
}
