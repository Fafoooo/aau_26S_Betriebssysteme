/*
 * Ü 2.5 - Dateizugriff in C (Schreiben)
 *
 * Wie users.c, aber die Ausgabe wird direkt in users2.csv geschrieben
 * anstatt auf stdout ausgegeben.
 *
 * Vergleich der beiden Dateien mit:
 *   diff users.csv users2.csv
 * oder:
 *   cmp users.csv users2.csv
 *
 * Zum Anhängen (append) statt Überschreiben:
 *   - In diesem Programm: fopen("users2.csv", "a") statt "w"
 *   - Mit Konsolenumleitung: ./users >> users.csv (doppeltes >>)
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX_LINE 1024

int main(void) {
    FILE *fp = fopen("/etc/passwd", "r");
    if (fp == NULL) {
        perror("Fehler beim Öffnen von /etc/passwd");
        return 1;
    }

    FILE *out = fopen("users2.csv", "w");
    if (out == NULL) {
        perror("Fehler beim Erstellen von users2.csv");
        fclose(fp);
        return 1;
    }

    char line[MAX_LINE];

    while (fgets(line, MAX_LINE, fp) != NULL) {
        /* Newline entfernen */
        char *nl = strchr(line, '\n');
        if (nl != NULL)
            *nl = '\0';

        /* Format: username:password:UID:GID:GECOS:home:shell */
        char *username = line;
        char *sep1 = strchr(line, ':');
        if (sep1 == NULL) continue;
        *sep1 = '\0';

        char *sep2 = strchr(sep1 + 1, ':');
        if (sep2 == NULL) continue;

        char *uid_str = sep2 + 1;
        char *sep3 = strchr(uid_str, ':');
        if (sep3 == NULL) continue;
        *sep3 = '\0';

        fprintf(out, "%s, %s\n", uid_str, username);
    }

    fclose(fp);
    fclose(out);
    return 0;
}
