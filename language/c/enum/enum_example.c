#include <stdio.h>

enum access_type{
	ACC_READ,
	ACC_WRITE
};

int main(int argc, char** argv)
{
	printf("ACC_WRITE=%d ACC_READ=%d\n"
			,ACC_WRITE
			,ACC_READ);
	printf("%s\n", ACC_WRITE ? "ACC_WRITE is true" : "ACC_WRITE is false");
	printf("%s\n", ACC_READ? "ACC_READ is true" : "ACC_READ is false");

}
