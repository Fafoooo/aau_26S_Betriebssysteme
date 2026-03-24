/* countif.c - function pointers */
#include <stdio.h>
#include <stdbool.h>

typedef bool (*IntFunction)(int);
static int prev = 0;

bool isEven(int v) { return v % 2 == 0; }

bool isGreaterThanPrevious(int v)
{
    bool r = v > prev;
    prev = v;
    return r;
}

unsigned count_if(int a[], unsigned n, IntFunction f)
{
    unsigned c = 0;
    for (unsigned i = 0; i < n; i++)
        if (f(a[i])) c++;
    return c;
}

int main(void)
{
    int vals[10] = {55,12,98,55,15,8,52,71,12,11};
    unsigned e = count_if(vals, 10, isEven);
    prev = 0;
    unsigned g = count_if(vals, 10, isGreaterThanPrevious);
    printf("isEven: %u\t\tisGreaterThanPrevious: %u\n", e, g);
    return 0;
}
