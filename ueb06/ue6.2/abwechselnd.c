// Ü 6.2 - Zwei Prozesse teilen sich data.txt
//
// Eltern und Kind reichen sich die Datei abwechselnd zu:
//   1. Eltern schreibt einen Text in data.txt
//   2. Eltern signalisiert: "Kind dran"
//   3. Kind liest, invertiert, schreibt zurueck
//   4. Kind signalisiert: "Eltern dran"
//   5. Eltern liest invertiert, gibt aus, loescht die Datei
// Wird 5x wiederholt.
//
// Synchronisation: zwei anonyme Pipes als Token-Kanaele.
// to_child  weckt das Kind, to_parent  weckt den Eltern.
//
// Kompilieren:  gcc -o abwechselnd abwechselnd.c

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

#define ITER 5
#define FILENAME "data.txt"

static void write_file(const char *text)
{
    FILE *f = fopen(FILENAME, "w");
    fputs(text, f);
    fclose(f);
}

static size_t read_file(char *buf, size_t len)
{
    FILE *f = fopen(FILENAME, "r");
    size_t n = fread(buf, 1, len - 1, f);
    fclose(f);
    buf[n] = 0;
    return n;
}

static void reverse(char *s, size_t n)
{
    for (size_t i = 0; i < n / 2; i++) {
        char t = s[i];
        s[i] = s[n - 1 - i];
        s[n - 1 - i] = t;
    }
}

int main(void)
{
    int to_child[2], to_parent[2];
    pipe(to_child);
    pipe(to_parent);

    if (fork() == 0) {
        // === Kind ===
        close(to_child[1]); close(to_parent[0]);
        char buf[256], tok;

        for (int i = 0; i < ITER; i++) {
            read(to_child[0], &tok, 1);          // warten
            size_t n = read_file(buf, sizeof(buf));
            reverse(buf, n);
            write_file(buf);
            write(to_parent[1], "x", 1);         // Eltern wecken
        }
        exit(EXIT_SUCCESS);
    }

    // === Eltern ===
    close(to_child[0]); close(to_parent[1]);
    char text[64], buf[256], tok;

    for (int i = 0; i < ITER; i++) {
        snprintf(text, sizeof(text),
                 "Iteration %d: HelloWorld - genug Zeichen!", i + 1);
        write_file(text);
        printf("[Eltern %d] schrieb:    \"%s\"\n", i + 1, text);

        write(to_child[1], "x", 1);              // Kind wecken
        read(to_parent[0], &tok, 1);             // auf Kind warten

        read_file(buf, sizeof(buf));
        printf("[Eltern %d] invertiert: \"%s\"\n", i + 1, buf);
        unlink(FILENAME);
    }

    wait(NULL);
    return EXIT_SUCCESS;
}
