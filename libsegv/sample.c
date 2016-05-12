#include <stdio.h>

void baz() {
	int *foo = (int*)-1;
	printf("%d\n", *foo);
}

void bar() { baz(); }
void foo() { bar(); }

int main(int argc, char **argv) {
	foo(); // this will call foo, bar, and baz.  baz segfaults.
	return 0;
}

