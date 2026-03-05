#include <stdio.h>

int main(void) {
    int prim[1001];

    // Alles erstmal als prim markieren
    for (int i = 0; i <= 1000; i++)
        prim[i] = 1;

    // 0 und 1 sind keine Primzahlen
    prim[0] = 0;
    prim[1] = 0;

    // Sieb: Vielfache streichen
    for (int i = 2; i * i <= 1000; i++) {
        if (prim[i]) {
            for (int j = i * i; j <= 1000; j += i)
                prim[j] = 0;
        }
    }

    // Ausgabe
    for (int i = 0; i <= 1000; i++) {
        printf("%d", i);
        if (i % 2 == 0)
            printf("x");
        if (prim[i])
            printf("p");
        printf("\n");
    }

    return 0;
}
