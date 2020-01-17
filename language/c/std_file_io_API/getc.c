#include <stdio.h>
#include <string.h>

int main(int argc, char **argv)
{
	FILE *stream_in;
	int c;

	if (argc==1)
	{
		stream_in = stdin;
	}
	else if (argc==2)
	{
		stream_in = fopen(argv[1], "r");
		if (stream_in==NULL)
		{
			perror("file open error: ");
			return 1;
		}
	}

	while ((c = getc(stream_in))!=EOF)
	{
		printf("%c", c);
	}
	if (argc==2)
	{
		fclose(stream_in);
	}
	return 0;
}

