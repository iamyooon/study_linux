#include <stdio.h>
#include <string.h>

int main()
{
	FILE *fp;

	char str[100] = "feof test string\n";

	fp = fopen("../feof.txt", "w");
	fputs("feof test string", fp);

	fclose(fp);
	fp = fopen("../feof.txt", "r");

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
