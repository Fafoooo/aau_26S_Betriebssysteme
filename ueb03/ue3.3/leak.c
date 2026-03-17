// Ü 3.3a - Memory Management: Speicherleck
//
// Zeigt was passiert wenn man in einer Endlosschleife malloc() aufruft
// ohne free(). Zwei Loesungsansaetze implementiert:
//   1) Error-Handling: Pruefen ob malloc NULL zurueckgibt
//   2) Speicherfreigabe: free() nach jeder Nutzung

#include <stdio.h>
#include <stdlib.h>

int main(int argc, char* argv[])
{
    unsigned long count = 0;

    while (1)
    {
        int *p = (int*) malloc(sizeof(int) * 1024);  // 4 KB pro Iteration

        // Loesung 1: Error-Handling - abbrechen wenn kein Speicher mehr da
        if (p == NULL)
        {
            printf("malloc fehlgeschlagen nach %lu Allokationen\n", count);
            printf("Das waren ca. %lu MB\n", (count * sizeof(int) * 1024) / (1024 * 1024));
            break;
        }

        *p = 42;  // Speicher benutzen
        count++;

        // Loesung 2: Speicher sofort wieder freigeben (kein Leak)
        free(p);

        // Alle 1 Million Iterationen Statusmeldung
        if (count % 1000000 == 0)
        {
            printf("%lu Millionen Allokationen (kein Leak dank free)\n", count / 1000000);
        }
    }

    return 0;
}
