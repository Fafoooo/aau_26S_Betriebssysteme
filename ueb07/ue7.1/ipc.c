// Ü 7.1 - Interprozesskommunikation mit Pipes
//
//
// === Aufgabe ===
//
// Ein C-Programm mit einem Hauptprozess und einem Kindprozess.
// Kommunikation über eine anonyme Pipe.
// Das Kind schreibt mehrere Nachrichten an den Eltern,
// der Eltern liest und gibt sie nummeriert aus.
//
//
// === Was ist eine Pipe? ===
//
// Eine Pipe ist ein unidirektionaler Byte-Stream-Kanal zwischen zwei
// verwandten Prozessen. Der Systemaufruf
//
//     int pipe(int fd[2]);
//
// erzeugt zwei File-Descriptoren:
//
//     fd[0]  - Lese-Ende
//     fd[1]  - Schreib-Ende
//
// Wird pipe() VOR fork() aufgerufen, erbt das Kind beide Enden, und
// damit haben Eltern und Kind einen gemeinsamen Kommunikationskanal.
// Genau das nutzen wir hier.
//
//
// === Wer schließt was? ===
//
// Best Practice: jedes Ende wird nur dort offen gehalten, wo es auch
// wirklich gebraucht wird. Wir machen es also so:
//
//     Kind:    close(fd[0])  -> nutzt nur Schreib-Ende
//     Eltern:  close(fd[1])  -> nutzt nur Lese-Ende
//
// Wichtig: solange irgendein Prozess das Schreib-Ende noch offen hat,
// gibt read() KEIN EOF zurück. Wenn der Eltern fd[1] nicht schließt,
// blockiert sein read() ewig nachdem das Kind fertig ist.
//
//
// === Nachrichten-Framing ===
//
// Pipes sind Byte-Streams - sie kennen keine Nachrichten-Grenzen.
// Wir trennen die Nachrichten mit dem Null-Byte ('\0'), das
// strlen() ohnehin schon mitliefert. Der Eltern liest alles bis EOF
// in einen Buffer und teilt dann an den '\0'-Stellen.
//
//
// === Beispielausgabe ===
//
//     write from 764: hello
//     write from 764: hello 2
//     write from 764: hello 3
//     message #1 read by 763: hello
//     message #2 read by 763: hello 2
//     message #3 read by 763: hello 3

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

int main(void)
{
    int fd[2];
    if (pipe(fd) == -1)
    {
        perror("pipe");
        exit(EXIT_FAILURE);
    }

    pid_t pid = fork();
    if (pid == -1)
    {
        perror("fork");
        exit(EXIT_FAILURE);
    }

    if (pid == 0)
    {
        // === Kindprozess ===
        // Lese-Ende nicht gebraucht, sofort schließen.
        close(fd[0]);

        const char *msgs[] = { "hello", "hello 2", "hello 3" };
        int count = sizeof(msgs) / sizeof(msgs[0]);

        for (int i = 0; i < count; i++)
        {
            printf("write from %d: %s\n", getpid(), msgs[i]);

            // strlen()+1, damit das '\0' als Trenner mitgeschrieben wird.
            if (write(fd[1], msgs[i], strlen(msgs[i]) + 1) == -1)
            {
                perror("write");
                exit(EXIT_FAILURE);
            }
        }

        // Schreib-Ende schließen, damit der Eltern beim Lesen ein EOF kriegt.
        close(fd[1]);
        exit(EXIT_SUCCESS);
    }

    // === Elternprozess ===
    // Schreib-Ende nicht gebraucht, sofort schließen.
    close(fd[1]);

    // Alles in einen Buffer einlesen, bis read() 0 (EOF) liefert.
    char buf[1024];
    ssize_t total = 0;
    ssize_t n;

    while ((n = read(fd[0], buf + total, sizeof(buf) - total)) > 0)
    {
        total += n;
    }
    close(fd[0]);

    // Buffer enthält jetzt alle Nachrichten, durch '\0' getrennt.
    // Wir laufen einfach durch und nutzen jeden String bis zum nächsten '\0'.
    char *p = buf;
    int i = 0;
    while (p < buf + total)
    {
        i++;
        printf("message #%d read by %d: %s\n", i, getpid(), p);
        p += strlen(p) + 1;
    }

    // Auf Kind warten, damit kein Zombie übrig bleibt.
    wait(NULL);
    return EXIT_SUCCESS;
}
