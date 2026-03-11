/*
 * Ü 2.4 - Dateizugriff in C (Lesen)
 *
 * /etc/passwd enthält Informationen über alle Benutzer des Systems.
 * Jede Zeile hat das Format: username:password:UID:GID:GECOS:home:shell
 * Die Felder sind durch ':' getrennt.
 *
 * /etc/shadow enthält die gehashten Passwörter der Benutzer.
 * Format: username:password_hash:lastchg:min:max:warn:inactive:expire
 * Diese Datei ist nur für root lesbar (Sicherheit).
 *
 * Das Kommandozeilentool man kann verwendet werden, um Informationen
 * zu diesen Dateien zu erhalten:
 *   man 5 passwd     (Dateiformat von /etc/passwd)
 *   man 5 shadow     (Dateiformat von /etc/shadow)
 * Die "5" steht für Sektion 5 (Dateiformate), um nicht die passwd(1)-
 * Manpage (das Kommando zum Passwort-Ändern) zu bekommen.
 *
 * Dieses Programm liest /etc/passwd und gibt für jeden Benutzer
 * die User-ID und den Benutzernamen im Format "{ID}, {Name}" aus.
 * Es verwendet strchr() zum Finden der Trennzeichen.
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

    char line[MAX_LINE];

    while (fgets(line, MAX_LINE, fp) != NULL) {
        /* Newline entfernen */
        char *nl = strchr(line, '\n');
        if (nl != NULL)
            *nl = '\0';

        /* Format: username:password:UID:GID:GECOS:home:shell */
        /* Feld 1: username (von Anfang bis zum ersten ':') */
        char *username = line;
        char *sep1 = strchr(line, ':');
        if (sep1 == NULL) continue;
        *sep1 = '\0';

        /* Feld 2: password (überspringen) */
        char *sep2 = strchr(sep1 + 1, ':');
        if (sep2 == NULL) continue;

        /* Feld 3: UID */
        char *uid_str = sep2 + 1;
        char *sep3 = strchr(uid_str, ':');
        if (sep3 == NULL) continue;
        *sep3 = '\0';

        printf("%s, %s\n", uid_str, username);
    }

    fclose(fp);
    return 0;
}
