#include <stdio.h>

int main()
{
	FILE *pFile = NULL;

	pFile = fopen("rewind.txt", "w+");
	if(pFile==NULL)
	{
		// error
	}
	else
	{
		printf("%1ld\n", ftell(pFile));

		fputs("0123456789", pFile);
		printf("%1ld\n", ftell(pFile));

		rewind(pFile);
		printf("%1ld\n", ftell(pFile));

		fclose(pFile);
	}
	return 0;
}
