// Ü 3.3a - Speicherleck
// Loesungsansaetze: 1) Error-Handling (malloc auf NULL pruefen)
//                   2) Speicherfreigabe (free nach Benutzung)
// Hier implementiert: Error-Handling

#include <stdio.h>
#include <stdlib.h>

int main(void)
{
    unsigned long count = 0;

    while (1)
    {
        int *p = malloc(sizeof(int) * 1024);
        if (p == NULL)
        {
            printf("malloc fehlgeschlagen nach %lu Allokationen\n", count);
            break;
        }
        count++;
    }

    return 0;
}
