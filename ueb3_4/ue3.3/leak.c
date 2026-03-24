/* leak.c - memory leak demonstration with error handling */

#include <stdio.h>
#include <stdlib.h>

int main(void)
{
    unsigned long n = 0;

    for (;;)
    {
        int *p = malloc(sizeof(int) * 1024);
        if (!p)
        {
            printf("malloc failed after %lu allocations\n", n);
            break;
        }
        *p = 1;
        free(p);
        n++;
    }

    return 0;
}
