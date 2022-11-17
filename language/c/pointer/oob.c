#include <stdio.h>

int main(int argc, char** argv)
{
	char** p_arg = NULL;

        for(p_arg = argv+1; *p_arg; p_arg++) {
		printf("hello %p\n",*(p_arg+1));
        }
	return 0;
}
