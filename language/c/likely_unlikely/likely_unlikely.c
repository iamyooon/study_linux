#include <stdio.h>
#include <inttypes.h>
#include <time.h>
#include <assert.h>

/*
 * Interesting likely, unlikely exmaple From stacckoverflow
 * https://stackoverflow.com/questions/1668013/can-likely-unlikely-macros-be-used-in-user-space-code
 *
 */

#ifdef __GNUC__
#define likely(x)       __builtin_expect(!!(x), 1)
#define unlikely(x)     __builtin_expect(!!(x), 0)
#else
#define likely(x)       (x)
#define unlikely(x)     (x)
#endif

uint64_t fib(uint64_t n)
{
    if (opt(n == 0 || n == 1)) {
        return n;
    } else {
        return fib(n - 2) + fib(n - 1);
    }
}

int main(int argc, char **argv)
{
    int i, max = 45;
    clock_t tm;

    if (argc == 2) {
        max = atoi(argv[1]);
        assert(max > 0);
    } else {
        assert(argc == 1);
    }

    tm = -clock();
    for (i = 0; i <= max; ++i)
        printf("fib(%d) = %" PRIu64 "\n", i, fib(i));
    tm += clock();

    printf("Time elapsed: %.3fs\n", (double)tm / CLOCKS_PER_SEC);
    return 0;
}
