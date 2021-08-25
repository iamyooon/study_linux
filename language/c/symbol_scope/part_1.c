#include <stdio.h>

int func1(void)
{
	printf("%d in %s\n", var_main, __func__);
}
