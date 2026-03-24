// endless.c - infinite loop demo
#include <stdio.h>

int main()
{
    unsigned long long n = 0;
    
    printf("Endless loop started. Use 'ulimit -t 4' to limit to 4 seconds.\n");
    
    while(1)
    {
        n++;
        if(n % 50000000 == 0)
        {
            printf("Count: %llu\n", n);
        }
    }
    
    return 0;
}