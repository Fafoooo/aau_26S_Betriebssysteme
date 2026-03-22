// Ü 3.3b - Maximale Groesse eines Integer-Arrays

#include <stdio.h>
#include <stdlib.h>

int main(void)
{
    size_t low = 1, high = 1;

    // Obere Grenze finden
    while (1)
    {
        int *p = malloc(sizeof(int) * high);
        if (p == NULL)
            break;
        free(p);
        low = high;
        high *= 2;
    }

    // Binaere Suche
    while (low + 1 < high)
    {
        size_t mid = low + (high - low) / 2;
        int *p = malloc(sizeof(int) * mid);
        if (p != NULL)
        {
            free(p);
            low = mid;
        }
        else
            high = mid;
    }

    printf("Maximale Array-Groesse: %zu Elemente\n", low);
    printf("Das entspricht ca. %.2f GB\n",
           (double)(low * sizeof(int)) / (1024.0 * 1024.0 * 1024.0));

    return 0;
}
