#include <stdio.h>

int main()
{
	char c;
	FILE *stream;

	stream = fopen("ungetc.txt", "r");

	while(1)
	{
		c = fgetc(stream);
		if(c==EOF)
		{
			perror("fgetc");
			break;
		}
		else if (c=='#')
		{
			ungetc('%', stream);
			ungetc('*', stream);
			ungetc('^', stream);
		}
		else
		{
			printf("%c", c);
		}
	}
	return 0;
}

