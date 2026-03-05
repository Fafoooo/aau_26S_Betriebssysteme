#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <time.h>

#define ARRAY_SIZE 8

void printArray(const uint8_t *arr, int size, int start) {
    if (arr == NULL || start < 0 || start >= size)
        return;

    for (int i = start; i < size; i++) {
        printf("F[%d] = %u\n", i, arr[i]);
    }
}

int main(void) {
    uint8_t fib[ARRAY_SIZE];

    fib[0] = 0;
    fib[1] = 1;
    for (int i = 2; i < ARRAY_SIZE; i++) {
        fib[i] = fib[i - 1] + fib[i - 2];
    }

    srand(time(NULL));
    int index = rand() % ARRAY_SIZE;
    printf("Zufaelliger Index: %d\n\n", index);

    printArray(fib, ARRAY_SIZE, index);

    return 0;
}
