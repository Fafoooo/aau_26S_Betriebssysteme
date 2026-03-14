// Ü 2.4 - Dateizugriff in C (Lesen)
//
// /etc/passwd: Infos über alle Benutzer, Format: username:password:UID:GID:GECOS:home:shell cat /etc/passwd
// /etc/shadow: gehashte Passwörter, nur fuer root lesbar cat /etc/shadow
// Info dazu mit: man 5 passwd / man 5 shadow (5 = Sektion Dateiformate)

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

    char line[MAXCHARS];

    // Zeile fuer Zeile lesen: username:password:UID:GID:...
    while (fgets(line, MAXCHARS, file) != NULL)
    {
        char *username = line;

        // Erstes ':' finden -> Ende vom Username
        char *sep1 = strchr(line, ':');
        if (sep1 == NULL) continue;
        *sep1 = '\0';

        // Zweites ':' finden -> Ende vom Password (ueberspringen)
        char *sep2 = strchr(sep1 + 1, ':');
        if (sep2 == NULL) continue;

        // Drittes ':' finden -> Ende der UID
        char *uid = sep2 + 1;
        char *sep3 = strchr(uid, ':');
        if (sep3 == NULL) continue;
        *sep3 = '\0';

        printf("%s, %s\n", uid, username);
    }

    fclose(file);
    return 0;
}
