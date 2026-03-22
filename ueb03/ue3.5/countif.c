// Ü 3.5 - Function Pointer

#include <stdio.h>
#include <stdbool.h>

typedef bool (*IntFunction)(int);

static int previous = 0;

bool isEven(int value)
{
    return value % 2 == 0;
}

bool isGreaterThanPrevious(int value)
{
    bool result = value > previous;
    previous = value;
    return result;
}

unsigned count_if(int array[], unsigned size, IntFunction func)
{
    unsigned count = 0;
    for (unsigned i = 0; i < size; i++)
    {
        if (func(array[i]))
            count++;
    }
    return count;
}

int main(void)
{
    int values[10] = {55, 12, 98, 55, 15, 8, 52, 71, 12, 11};

    unsigned even = count_if(values, 10, isEven);
    previous = 0;
    unsigned greater = count_if(values, 10, isGreaterThanPrevious);

    printf("isEven: %u\t\tisGreaterThanPrevious: %u\n", even, greater);
    return 0;
}
