#include <stdio.h>

#define __GFP_MOVABLE 0x8
#define __GFP_CMA 0x10
int main(int argc, char** argv)
{
	int gfp_flags = __GFP_MOVABLE | __GFP_CMA;
	printf("%d\n",((gfp_flags & __GFP_MOVABLE) != 0) << 1);
	printf("%d\n",((gfp_flags & __GFP_CMA) != 0) << 1);
	return 0;
}
