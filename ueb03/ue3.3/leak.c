// Ü 3.3a - Memory Management: Memory Leak
//
// Endlosschleife mit malloc -> beobachten mit top/htop
//
// Loesung 1: Error-Handling -> pruefen ob malloc NULL zurueckgibt
// Loesung 2: Speicherfreigabe -> free() nach Verwendung
// Hier: beide Ansaetze implementiert

#include <stdio.h>
#include <stdlib.h>

int main(int argc, char* argv[])
{
    unsigned long long count = 0;
    int *ptr;

    printf("Memory Leak Demo (mit Fix)\n");
    printf("Beobachten mit: top -p $(pgrep leak)\n\n");

    while (1)
    {
        // Speicher allokieren
        ptr = (int*) malloc(sizeof(int));

        // Loesung 1: Error-Handling - pruefen ob malloc erfolgreich war
        if (ptr == NULL)
        {
            printf("malloc fehlgeschlagen nach %llu Allokationen!\n", count);
            printf("Kein Speicher mehr verfuegbar.\n");
            break;
        }

        *ptr = 42;
        count++;

        // Loesung 2: Speicherfreigabe - free() nach Verwendung
        // Ohne free() wuerde der Speicherverbrauch endlos steigen (Memory Leak)
        free(ptr);

        if (count % 10000000 == 0)
        {
            printf("%llu Allokationen (Speicher stabil dank free)\n", count);
        }
    }

    return 0;
}
