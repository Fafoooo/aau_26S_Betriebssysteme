// Ü 3.3b - Maximale Groesse eines Integer-Arrays finden
//
// Strategie: Mit grossem Block anfangen, bei Fehler halbieren,
// dann per Binaersuche die genaue Grenze finden.

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

int main(int argc, char* argv[])
{
    size_t low = 0;
    size_t high = SIZE_MAX / sizeof(int);  // Theoretisches Maximum
    size_t best = 0;
    int *ptr;

    printf("Suche maximale int-Array-Groesse...\n\n");

    // Erst mal eine obere Grenze finden die fehlschlaegt
    size_t test = 1;
    while (test < high)
    {
        ptr = (int*) malloc(test * sizeof(int));
        if (ptr == NULL)
        {
            high = test;
            break;
        }
        free(ptr);
        best = test;
        low = test;
        test *= 2;
    }

    // Binaersuche zwischen low und high
    while (low + 1 < high)
    {
        size_t mid = low + (high - low) / 2;
        ptr = (int*) malloc(mid * sizeof(int));
        if (ptr != NULL)
        {
            free(ptr);
            best = mid;
            low = mid;
        }
        else
        {
            high = mid;
        }
    }

    printf("Maximale Array-Groesse: %zu Elemente\n", best);
    printf("Das entspricht:         %zu Bytes (%.2f GB)\n",
           best * sizeof(int),
           (double)(best * sizeof(int)) / (1024.0 * 1024.0 * 1024.0));

    return 0;
}
