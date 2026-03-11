/*
 * Ü 2.3 - Funktionen mit Pointern
 *
 * Zwei int-Variablen A und B werden mit 0 initialisiert.
 * myFunc() erhält Pointer auf A und B, inkrementiert beide um 10.
 * Durch die Übergabe als Pointer (call by reference) werden die
 * Originalwerte in main() verändert.
 */

#include <stdio.h>
#include "myfunc.h"

/*
 * myFunc: Inkrementiert die Werte, auf die a und b zeigen, um jeweils 10.
 * Durch Pointer-Übergabe (call by reference) werden die Originalvariablen
 * in main() direkt verändert.
 */
void myFunc(int *a, int *b) {
    *a += 10;
    *b += 10;
    printf("In myFunc: A = %d, B = %d\n", *a, *b);
}

int main(void) {
    int A = 0;
    int B = 0;

    printf("Vor myFunc: A = %d, B = %d\n", A, B);

    /*
     * Wir übergeben die ADRESSEN von A und B (&A, &B).
     * So kann myFunc() die Originalwerte verändern (call by reference).
     * Ohne Pointer (call by value) würde myFunc() nur Kopien ändern
     * und A,B blieben in main() bei 0.
     */
    myFunc(&A, &B);

    printf("Nach myFunc: A = %d, B = %d\n", A, B);

    return 0;
}
