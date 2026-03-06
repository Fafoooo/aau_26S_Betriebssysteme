#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <time.h>

int main(void) {
    uint8_t fib[8];
    int index;

    fib[0] = 0;
    fib[1] = 1;

    for (int i = 2; i < 8; i++) {
        fib[i] = fib[i - 1] + fib[i - 2];
    }

    srand(time(NULL)); 
    index = rand() % 8; // Zufälliger Index zwischen 0 und 7

    printf("Zufaelliger Index: %d\n\n", index);

    // Ausgabe der Fibonacci-Zahlen ab dem zufälligen Index
    for (int i = index; i < 8; i++) {
        printf("F[%d] = %u\n", i, fib[i]);
    }

    return 0;
}