#include <stdio.h>
#include <string.h>

#define __FILENAME__ (strrchr(__FILE__, '/') ? strrchr(__FILE__, '/') + 1 : __FILE__)

int main(int argc, char** argv)
{
	printf("original __FILE__=%s\n", __FILE__);
	printf("short version __FILE__=%s\n", __FILENAME__);
	return 0;
}

