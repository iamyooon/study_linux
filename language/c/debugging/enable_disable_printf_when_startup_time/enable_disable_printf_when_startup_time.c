/*
 * ref. https://stackoverflow.com/questions/5765175/macro-to-turn-off-printf-statements
 */
#include <stdio.h>
#include <stdbool.h>

bool disable_printf = false;

#define my_printf(...) \
	({ \
	 if (!disable_printf) \
		printf(__VA_ARGS__); \
	 })

int main(int argc, char** argv)
{
	if (argc > 1)
		disable_printf = true;
	my_printf("you don't use print disable\n");
	my_printf("if you want to disable printf, run with arguement\n");
}
