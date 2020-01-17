#include <stdio.h>

int main()
{
	char ch_key;

	while(1)
	{
		ch_key = getchar();
		if (EOF==ch_key)
		{
			printf("exit");
			break;
		}
		else 
		{
			putchar(ch_key);
		}
	}
	return 0;
}
