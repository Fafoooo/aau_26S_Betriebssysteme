/* countif.c - function pointer exercise */

#include <stdio.h>
#include <stdbool.h>

typedef bool (*IntFunction)(int);

static int prev_val = 0;

bool isEven(int x)
{
    return x % 2 == 0;
}

bool isGreaterThanPrevious(int x)
{
    bool r = x > prev_val;
    prev_val = x;
    return r;
}

unsigned count_if(int arr[], unsigned len, IntFunction fn)
{
    unsigned count = 0;
    for (unsigned i = 0; i < len; i++)
        if (fn(arr[i]))
            count++;
    return count;
}

int main(void)
{
    int values[] = {55, 12, 98, 55, 15, 8, 52, 71, 12, 11};
    unsigned n = sizeof(values) / sizeof(values[0]);

    printf("isEven: %u\t\tisGreaterThanPrevious: ",
           count_if(values, n, isEven));

    prev_val = 0;
    printf("%u\n", count_if(values, n, isGreaterThanPrevious));

    return 0;
}
