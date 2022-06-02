#include <stdio.h>

struct test_struct {
	int a;
	int b;
	int c;
	int d;
};

int main(int argc, char** argv)
{
	int arr[4];
	int *parr;
	struct test_struct* ptr;

	parr = arr;

	printf("arr[0]=%d,arr[1]=%d,arr[2]=%d,arr[3]=%d\n", arr[0], arr[1], arr[2], arr[3]);
	printf("%d,%d,%d,%d\n", parr[0], parr[1], parr[2], parr[3]);

	arr[0] = 100;
	arr[1] = 200;
	arr[2] = 300;
	arr[3] = 400;

	printf("%d,%d,%d,%d\n", arr[0], arr[1], arr[2], arr[3]);
	printf("%d,%d,%d,%d\n", parr[0], parr[1], parr[2], parr[3]);


	ptr = (struct test_struct *)arr;

	printf("%d,%d,%d,%d\n", ptr->a, ptr->b, ptr->c, ptr->d);
	printf("%llx,%llx,%llx,%llx,%llx\n", ptr, &ptr->a, &ptr->b, &ptr->c, &ptr->d);

	ptr->a = 110;
	ptr->b = 210;
	ptr->c = 310;
	ptr->d = 410;

	printf("%d,%d,%d,%d\n", ptr->a, ptr->b, ptr->c, ptr->d);
	return 0;
}
