#include <stdlib.h>
void FooBar() {
	malloc(7);
}
int main() {
	FooBar();
	return 0;
}
