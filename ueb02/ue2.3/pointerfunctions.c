// Ü 2.3 - Funktionen mit Pointern

#include <stdio.h>
#include "myfunc.h"

void myFunc(int *a, int *b) {
    *a += 10;
    *b += 10;
    printf("In myFunc: A = %d, B = %d\n", *a, *b);
}

int main(void) {
    int A = 0;
    int B = 0;

    printf("Vor myFunc: A = %d, B = %d\n", A, B);

    myFunc(&A, &B);

    printf("Nach myFunc: A = %d, B = %d\n", A, B);

    return 0;
}
