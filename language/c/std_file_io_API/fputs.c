#include <stdio.h>
#include <string.h>

int main()
{
	FILE *fp;

	fp = fopen("fputs.txt", "w");
	fputs("hello\n", fp);

	fputs("fputs success\n", stdout);
	fclose(fp);
}
