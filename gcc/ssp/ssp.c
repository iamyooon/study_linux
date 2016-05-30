#include <stdio.h>
#include <string.h>

void protected_func(void)
{
	char buf[8];
	strncpy(buf,"abcdefghijklmnop",17);
}

int main(void)
{
	protected_func();
	return 0;
}
