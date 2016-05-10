#include <stdio.h>

int main(int argc, char** argv)
{
	typeof(printf) *p_printf;
	p_printf = printf;

	p_printf("p_printf's address is %p\n",p_printf);

	return 0;
}
