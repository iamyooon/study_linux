#include <stdio.h>

int main()
{
	FILE *pFile = NULL;

	pFile = fopen("fopen.txt", "w+t");
	if(pFile != NULL)
	{
		fputs("fopen test", pFile);
		fclose(pFile);
	}
	else
	{
		// error
	}
	return 0;
}

	
