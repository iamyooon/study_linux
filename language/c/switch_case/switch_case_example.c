#include <stdio.h>
#include <stdlib.h>

int main(int argc, char** argv)
{
	int val;

	if (argc < 2)
		return 0;

	val = atoi(argv[1]);

	switch (val) {
		// default가 앞에 있다고 먼저 실행되거나 default로
		// 모든 case에서 실행되는게 아님.
		default:
			printf("default : %d\n", val);
			break;
		case 1:
			printf("case 1 : %d\n", val);
			break;
		case 2:
			printf("case 2 : %d\n", val);
			break;
		case 3:
			printf("case 3 : %d\n", val);
			break;
		case 4:
			printf("case 4 : %d\n", val);
			break;
	}
}
