#include <stdio.h>

int main(int argc, char** argv)
{
	int i;
	int *p_i;
	int arr_i[4]={0,};
	int *arr_p_i[]={
		0,
		0,
		0,
		0,
	};

	char c;
	char *p_c;
	char arr_c[4]={0,};
	char *arr_p_c[]={
		"hello",
		"hi",
		"bye"
	};

	printf("%ld %ld %ld %ld %ld %ld %ld %ld %ld %ld %ld\n",
		sizeof(i),		//4
		sizeof(&i),		//8
		sizeof(p_i),		//8
		sizeof(arr_i),		//16
		sizeof(&arr_i),		//8
		sizeof(arr_i[0]),	//4
		sizeof(&arr_i[0]),	//8
		sizeof(arr_p_i),	//32
		sizeof(&arr_p_i),	//8
		sizeof(arr_p_i[0]),	//8
		sizeof(&arr_p_i[0]));	//8
	
	printf("%ld %ld %ld %ld %ld %ld %ld %ld %ld %ld %ld %ld %ld %ld\n",
		sizeof(c),		//1
		sizeof(&c),		//8
		sizeof(p_c),		//8
		sizeof(&p_c),		//8
		sizeof(arr_c),		//4
		sizeof(&arr_c),		//8
		sizeof(arr_c[0]),	//1
		sizeof(&arr_c[0]),	//8
		sizeof(arr_p_c),	//24
		sizeof(&arr_p_c),	//8
		sizeof(arr_p_c[0]),	//8
		sizeof(&arr_p_c[0]),	//8
		sizeof(arr_p_c[0][0]),	//1
		sizeof(&arr_p_c[0][0]));//8

	return 0;
}
