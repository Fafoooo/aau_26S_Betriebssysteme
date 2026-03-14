// Ü 2.5 - Dateizugriff in C (Schreiben)
//
// Wie users.c, aber Ausgabe geht in users2.csv statt auf die Konsole.
// Vergleich: diff users.csv users2.csv
// Append statt ueberschreiben: fopen("users2.csv", "a") statt "w"
// Oder in der Konsole: ./users >> users.csv (doppeltes >>)

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAXCHARS (200 + 1)

int main(int argc, char* argv[])
{
    FILE* file = fopen("/etc/passwd", "r");
    if (!file) {
        perror("Error opening file");
        return 1;
    }

    // "w" = schreiben, ueberschreibt falls Datei existiert
    FILE* out = fopen("users2.csv", "w");
    if (!out) {
        perror("Error creating users2.csv");
        fclose(file);
        return 1;
    }

    char line[MAXCHARS];

    while (fgets(line, MAXCHARS, file) != NULL)
    {
        char *username = line;

        char *sep1 = strchr(line, ':');
        if (sep1 == NULL) continue;
        *sep1 = '\0';

        char *sep2 = strchr(sep1 + 1, ':');
        if (sep2 == NULL) continue;

        char *uid = sep2 + 1;
        char *sep3 = strchr(uid, ':');
        if (sep3 == NULL) continue;
        *sep3 = '\0';

        // Einziger Unterschied zu users.c: fprintf statt printf
        fprintf(out, "%s, %s\n", uid, username);
    }

    fclose(file);
    fclose(out);
    return 0;
}
