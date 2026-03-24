// Ü 5.1 - argc und argv: Parameter einlesen, Zahlen erkennen, Summe berechnen
//
// argc = Anzahl der Argumente (inkl. Programmname)
// argv = Array von Strings (argv[0] = Programmname, argv[1] = erster Parameter, ...)
//
// Aufruf: ./argreader 1 2 nan 4
// Ausgabe:
//   Anzahl der Parameter: 4
//   1 Summe: 1
//   2 Summe: 3
//   nan
//   4 Summe: 7

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <errno.h>

// Prueft ob ein String eine gueltige Zahl ist
int is_number(const char *str)
{
    if (str == NULL || *str == '\0')
        return 0;

    char *endptr;
    errno = 0;
    strtol(str, &endptr, 10);

    // Erfolgreich wenn alles konsumiert wurde und kein Fehler
    return (errno == 0 && *endptr == '\0');
}

int main(int argc, char *argv[])
{
    printf("Anzahl der Parameter: %d\n", argc - 1);

    long sum = 0;

    for (int i = 1; i < argc; i++)
    {
        if (is_number(argv[i]))
        {
            long val = strtol(argv[i], NULL, 10);
            sum += val;
            printf("%s Summe: %ld\n", argv[i], sum);
        }
        else
        {
            printf("%s\n", argv[i]);
        }
    }

    return 0;
}
