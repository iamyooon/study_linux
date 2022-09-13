#include <stdio.h>
#include <string.h>
#include <unistd.h> //sleep

int main(int argc, char **argv)
{
	int cnt = 0;
	int ret;

	do {
		printf("begin\n");
		sleep(1);
		cnt++;

		if(cnt > 5) {
			printf("break in if() escape loop(), end msg is not shown\n");
			ret = 1;
			break;
		}

		printf("end\n");
	}while(!ret);

	cnt = 0;
	ret = 0;

	do {
		printf("begin\n");
		sleep(1);
		cnt++;

		if(cnt > 5) {
			printf("continue in if() goto begin of loop(), end msg is also not shown\n");
			ret = 0;
			continue;
		}

		printf("end\n");
	}while(!ret);

	return 0;
}
