// leak.c - memory leak demo
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main()
{
    int n = 0;
    char *p;
    
    printf("Memory leak demo - allocating 1MB blocks...\n");
    
    while((p = malloc(1024 * 1024)) != NULL)
    {
        memset(p, 0xFF, 1024 * 1024);
        n++;
        if(n % 50 == 0)
        {
            printf("Allocated %d MB\n", n);
        }
        // Missing free(p) - this is the leak!
    }
    
    printf("Allocation failed at %d MB\n", n);
    return 1;
}