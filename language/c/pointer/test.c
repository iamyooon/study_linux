#include <stdio.h>

struct str{
	int a;
	int b;
	char c[32];
};

int main(int argc, char** argv)
{
	struct str arr[32];
	struct str* p_arr;
	struct str* pp_arr[32]={0,};
	unsigned long *ptr=0;

	p_arr = 0;

	printf("%x %x\n", ptr, ptr+1);
	printf("%x %x\n", p_arr, arr);
	printf("%x %x\n", p_arr, p_arr+1);
	printf("%x %x\n", arr, arr+1);
	printf("%x %x %x\n", pp_arr, pp_arr+1, pp_arr[1]+1);

	return 0;
}
