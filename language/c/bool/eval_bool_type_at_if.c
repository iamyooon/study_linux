#include <stdio.h>
#include <stdbool.h>

bool test(int val)
{
	if (val)
		return true;
	else if (!val)
		return false;
	else  {
		printf("????\n");
		return false;
	}
}

int main(int argc, char** argv)
{
	printf("test(0) - %d\n", test(0));
	printf("test(1) - %d\n", test(1));
	return 0;
}

