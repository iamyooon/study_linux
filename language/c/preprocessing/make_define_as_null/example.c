#include <stdio.h>
#include <string.h>
#include <unistd.h> //sleep

/*#define ww_log2(fmt, ...) \
       printf("wewake," fmt , ##__VA_ARGS__)
*/

// https://stackoverflow.com/questions/59934823/meaning-of-null-definition-for-a-macro-in-c
#define ww_log2(fmt, ...) (NULL)

int main(int argc, char **argv)
{

	ww_log2("hello world\n");

	return 0;
}
