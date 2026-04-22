// Ü 5.1 - argc und argv: Parameter einlesen, Zahlen erkennen, Summe berechnen
//
//
// === Aufgabe ===
//
// Das Programm akzeptiert beliebig viele Parameter auf der Kommandozeile.
// Zuerst wird die Anzahl der Parameter ausgegeben, dann jeder Parameter
// in einer eigenen Zeile. Wenn ein Parameter eine Zahl ist, wird zusätzlich
// die laufende Summe aller bisher erhaltenen Zahlen danebengeschrieben.
//
// argc enthält die Anzahl der Argumente inklusive Programmname in argv[0].
// argv ist ein Array von Strings: argv[0] ist der Programmname, argv[1..]
// sind die übergebenen Parameter.
//
//
// === Beispiel ===
//
// Aufruf:   ./argreader 1 2 nan 4
// Ausgabe:  Anzahl der Parameter: 4
//           1 Summe: 1
//           2 Summe: 3
//           nan
//           4 Summe: 7

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

// Prüft, ob ein String eine gültige ganze Zahl ist.
// Erfolgreich, wenn strtol alles verarbeitet und kein Fehler auftritt.
int is_number(const char *str)
{
    if (str == NULL || *str == '\0')
        return 0;

    char *endptr;
    errno = 0;
    strtol(str, &endptr, 10);

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
            sum += strtol(argv[i], NULL, 10);
            printf("%s Summe: %ld\n", argv[i], sum);
        }
        else
        {
            printf("%s\n", argv[i]);
        }
    }

    return 0;
}
