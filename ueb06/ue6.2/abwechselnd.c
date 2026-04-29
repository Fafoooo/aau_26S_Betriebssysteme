// Ü 6.2 - Zwei Prozesse teilen sich data.txt
//
// Eltern und Kind reichen sich die Datei abwechselnd zu, 5 Mal:
//   1. Eltern schreibt einen Text in data.txt
//   2. Kind liest, invertiert (ABC -> CBA), schreibt zurück
//   3. Eltern liest, gibt aus, löscht die Datei
// Sync über zwei Pipes (Token weiterreichen).
//
// Kompilieren:  gcc -o abwechselnd abwechselnd.c

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

#define ITER 5
#define DATAFILE "data.txt"

// Datei mit Text überschreiben
static void write_file(const char *t) {
    FILE *f = fopen(DATAFILE, "w");      // zum Schreiben öffnen
    fputs(t, f);                          // String reinschreiben
    fclose(f);                            // Datei schließen
}

// Datei in buf einlesen, gibt Anzahl Bytes zurück
static size_t read_file(char *b, size_t l) {
    FILE *f = fopen(DATAFILE, "r");      // zum Lesen öffnen
    size_t n = fread(b, 1, l - 1, f);    // bis zu l-1 Bytes lesen
    fclose(f);                            // Datei schließen
    b[n] = 0;                             // Null-Terminator setzen
    return n;                             // Anzahl gelesener Bytes
}

// String in-place umdrehen: ABC -> CBA
static void reverse(char *s, size_t n) {
    for (size_t i = 0; i < n / 2; i++) {  // Zeichen vorne+hinten tauschen
        char t = s[i];                    // Anfang merken
        s[i] = s[n-1-i];                  // Ende nach vorne
        s[n-1-i] = t;                     // gemerkten Anfang nach hinten
    }
}

int main(void)
{
    int to_child[2], to_parent[2];        // 2 Pipes für Token-Sync
    pipe(to_child);                        // Eltern->Kind  
    pipe(to_parent);                       // Kind->Eltern

    if (fork() == 0) {                     // 0 = wir sind im Kind
        // === Kind ===
        char buf[256], tok;                // Lese-Buffer + Token-Byte

        for (int i = 0; i < ITER; i++) {   // 5 Durchläufe
            read(to_child[0], &tok, 1);    // warten bis Eltern schickt
            size_t n = read_file(buf, sizeof buf);  // Datei einlesen
            reverse(buf, n);               // umdrehen
            write_file(buf);               // zurückschreiben
            write(to_parent[1], "x", 1);   // Eltern wecken
        }
        exit(EXIT_SUCCESS);                // Kind sauber beenden
    }

    // === Eltern ===
    char text[64], buf[256], tok;          // Schreib-/Lese-Buffer + Token

    for (int i = 0; i < ITER; i++) {       // 5 Durchläufe
        snprintf(text, sizeof text,        // Text für diese Iteration bauen
                 "Iteration %d: HelloWorld - genug Zeichen!", i + 1);
        write_file(text);                  // Datei beschreiben
        printf("[Eltern %d] schrieb:    \"%s\"\n", i + 1, text);  // Log

        write(to_child[1], "x", 1);        // Kind wecken
        read(to_parent[0], &tok, 1);       // warten bis Kind fertig

        read_file(buf, sizeof buf);        // jetzt invertierten Text lesen
        printf("[Eltern %d] invertiert: \"%s\"\n", i + 1, buf);   // Log
        unlink(DATAFILE);                  // Datei löschen
    }
    wait(NULL);                            // auf Kind warten (kein Zombie)
    return EXIT_SUCCESS;
}
