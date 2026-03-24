// biggest.c - find max allocatable int array
#include <stdio.h>
#include <stdlib.h>

int main()
{
    unsigned long lo = 0, hi = 1000000000UL, mid, max = 0;
    int *p;
    
    printf("Finding max allocatable integers...\n");
    
    while(lo <= hi)
    {
        mid = lo + (hi - lo) / 2;
        p = malloc(mid * sizeof(int));
        
        if(p != NULL)
        {
            max = mid;
            free(p);
            lo = mid + 1;
        }
        else
        {
            hi = mid - 1;
        }
    }
    
    printf("Max integers: %lu\n", max);
    printf("Size: %lu bytes (%.2f MB)\n", max * sizeof(int), 
           (double)(max * sizeof(int)) / (1024 * 1024));
    
    return 0;
}