#include <stdio.h>
#include <string.h>

int main()
{
	FILE *fp = NULL;
	char str[255];

	fp = fopen("fgets.txt", "w");
	fputs("fgets test string", fp);

	fclose(fp);
	fp = fopen("fgets.txt", "r");

	if (fp!=NULL)
	{
		while(!feof(fp))
		{
			fgets(str, sizeof(str), fp);
			printf("%s\n", str);
		}
		fclose(fp);
	}
	else
	{
		// error
	}
	return 0;
}
