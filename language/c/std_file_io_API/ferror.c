#include <stdio.h>
#include <string.h>

int main()
{
	FILE *fp;

	char str[100] = "ferror test string\n";

	fp = fopen("../ferror.txt", "w");
	fputs("ferror test string", fp);
	fclose(fp);

	fp = fopen("../ferror.txt", "r");

	while(!feof(fp))
	{
		fgets(str, sizeof(str), fp);
		if (ferror(fp))
		{
			printf("read error!");
			break;
		}
		else
		{
			printf("%s\n", str);
		}
	}
	fclose(fp);
}
