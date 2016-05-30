#include <stdio.h>

void begin (void) __attribute__((constructor));
void end (void) __attribute__((destructor));

int main (void)
{
	printf ("\nInside main ()");
}

void begin (void)
{
	printf ("\nIn begin ()");
}

void end (void)
{
	printf ("\nIn end ()\n");
}
