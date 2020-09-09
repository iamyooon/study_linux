#include <stdio.h>
#include <string.h>

int main()
{
	int ch;
	while((ch = fgetc(stdin)) != EOF)
	{
		printf("%c", ch);
	}
	return 1;
}
