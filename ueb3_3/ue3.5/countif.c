/* countif.c - Function Pointer Aufgabe */
/* Uses defensive programming with macros */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define ARRAY_SIZE 10
#define INITIAL_PREV 0

typedef bool (*IntFunction)(int);

static int g_previous = INITIAL_PREV;

bool isEven(int value)
{
    if (value < 0) value = -value;  /* handle negative */
    return (value % 2) == 0;
}

bool isGreaterThanPrevious(int value)
{
    bool result = (value > g_previous);
    g_previous = value;
    return result;
}

unsigned count_if(int array[], unsigned size, IntFunction func)
{
    if (array == NULL || func == NULL || size == 0)
    {
        fprintf(stderr, "Error: invalid arguments to count_if\n");
        return 0;
    }

    unsigned cnt = 0;
    for (unsigned idx = 0; idx < size; idx++)
    {
        if (func(array[idx]))
            cnt++;
    }
    return cnt;
}

int main(void)
{
    int values[ARRAY_SIZE] = {55, 12, 98, 55, 15, 8, 52, 71, 12, 11};

    unsigned even_count = count_if(values, ARRAY_SIZE, isEven);

    /* Reset state before second test */
    g_previous = INITIAL_PREV;
    unsigned greater_count = count_if(values, ARRAY_SIZE, isGreaterThanPrevious);

    printf("isEven: %u\t\tisGreaterThanPrevious: %u\n", even_count, greater_count);

    return EXIT_SUCCESS;
}
