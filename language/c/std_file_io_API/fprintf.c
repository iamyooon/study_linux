#include <stdio.h>
#include <string.h>

int main()
{
	FILE *fp;
	char str[1024] = "fprintf test string";

	fp = fopen("fprintf.txt", "w");

	if (fp == NULL)
		return -1;
	
	fprintf(fp, "%s", str);

	fclose(fp);
}
