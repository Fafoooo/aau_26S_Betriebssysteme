#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#define NUM 11


int compute(int* arr, int len)
{
    int sum     = 0;
    int squares = 0;

    for (int i = 0; i < len; i++)
    {
        sum += arr[i];
        squares += arr[i] * arr[i];
    }
    int result = squares / sum;
    assert(result);
    return result;
}


int main(int argc, char** argv)
{
    int arr[NUM];

    for (int i = 0; i < NUM; ++i)
    {
        arr[i] = NUM / 2 - i;
    }

    printf("%d \n", compute(arr, NUM));

    return 0;
}
