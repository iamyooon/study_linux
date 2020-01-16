#include <stdio.h>

int main()
{
	FILE *pFile = NULL;

	pFile = fopen("fputc.txt", "w");
	if (pFile==NULL)
	{
		//error
	}
	else
	{
		int result = fputc('T', pFile);
		printf("%x %c\n", result, result);

		fclose(pFile);
	}
	return 0;
}
