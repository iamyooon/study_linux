#include <stdio.h>

int main()
{
	int c;
	FILE *stream;

	stream = fopen("putc.txt", "w");

	c = 'x';
	putc(c, stream);

	return 0;
}
