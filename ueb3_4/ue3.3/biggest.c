/* biggest.c - find maximum allocatable int array */

#include <stdio.h>
#include <stdlib.h>

int try_alloc(size_t n)
{
    int *p = malloc(sizeof(int) * n);
    if (!p) return 0;
    free(p);
    return 1;
}

int main(void)
{
    size_t lo = 1, hi = 1;

    while (try_alloc(hi))
    {
        lo = hi;
        hi *= 2;
    }

    while (lo + 1 < hi)
    {
        size_t mid = lo + (hi - lo) / 2;
        if (try_alloc(mid))
            lo = mid;
        else
            hi = mid;
    }

    printf("Max: %zu elements (%.2f GB)\n", lo,
           (double)(lo * sizeof(int)) / (1024.0 * 1024.0 * 1024.0));

    return 0;
}
