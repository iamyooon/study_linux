#include <stdio.h>
void protected_func(void)
{
	char buf[8];
}

int main(void)
{
	protected_func();
	return 0;
}
