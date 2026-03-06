#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <time.h>

void printArray(const uint8_t *arr, int size, int start) {
    if (arr == NULL || start < 0 || start >= size)
        return;

    for (int i = start; i < size; i++) {
        printf("F[%d] = %u\n", i, arr[i]);
    }
}

int main(void) {
    int size = 8;
    uint8_t fib[size];
    int index;

    fib[0] = 0;
    fib[1] = 1;

    for (int i = 2; i < size; i++) {
        fib[i] = fib[i - 1] + fib[i - 2];
    }

    srand(time(NULL));
    index = rand() % size; // Zufälliger Index zwischen 0 und size-1

    printf("Zufaelliger Index: %d\n\n", index);

    printArray(fib, size, index);

    return 0;
}