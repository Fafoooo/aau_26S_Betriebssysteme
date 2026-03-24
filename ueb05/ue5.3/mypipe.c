// Ü 5.3 - Interprozesskommunikation: Pipes
//
// Kind erzeugt zufaellige int-Werte und schickt sie per Pipe an den Elternprozess.
// Elternprozess berechnet den Durchschnitt.
// Trick: Zuerst die Anzahl der Werte senden, damit der Parent weiss wie viele kommen.

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>

#define READ  0
#define WRITE 1

int main(int argc, char **argv)
{
    int   thePipe[2];
    pid_t pid;

    // Schritt 1: Pipe erstellen
    if (pipe(thePipe) == -1)
    {
        perror("pipe");
        return EXIT_FAILURE;
    }

    // Schritt 2: Fork
    pid = fork();
    if (pid < (pid_t)0)
    {
        fprintf(stderr, "Fork failed.\n");
        return EXIT_FAILURE;
    }

    if (pid == (pid_t)0)
    {
        // === KINDPROZESS ===

        // Lese-Ende schliessen (Kind schreibt nur)
        close(thePipe[READ]);

        // Zufallsgenerator initialisieren
        srand(time(NULL) ^ getpid());

        // Zufaellige Anzahl an Werten (zwischen 3 und 10)
        int count = 3 + rand() % 8;

        // Zuerst die Anzahl senden
        write(thePipe[WRITE], &count, sizeof(int));
        printf("[Kind PID %d] Sende %d Werte:\n", getpid(), count);

        // Dann die einzelnen Werte senden
        for (int i = 0; i < count; i++)
        {
            int value = rand() % 100;  // Zufallszahl 0-99
            printf("[Kind] Sende Wert %d: %d\n", i + 1, value);
            write(thePipe[WRITE], &value, sizeof(int));
            sleep(rand() % 2);  // Kurze Pause fuer Lesbarkeit
        }

        // Schreib-Ende schliessen und beenden
        close(thePipe[WRITE]);
        exit(EXIT_SUCCESS);
    }

    // === ELTERNPROZESS ===

    // Schreib-Ende schliessen (Eltern liest nur)
    close(thePipe[WRITE]);

    // Zuerst die Anzahl lesen
    int count;
    read(thePipe[READ], &count, sizeof(int));
    printf("[Eltern PID %d] Erwarte %d Werte:\n", getpid(), count);

    // Werte empfangen und summieren
    long sum = 0;
    int  value;
    for (int i = 0; i < count; i++)
    {
        read(thePipe[READ], &value, sizeof(int));
        sum += value;
        printf("[Eltern] Empfange Wert %d: %d\n", i + 1, value);
    }

    // Durchschnitt berechnen
    double avg = (double)sum / count;
    printf("\n[Eltern] Summe: %ld, Anzahl: %d, Durchschnitt: %.2f\n", sum, count, avg);

    // Lese-Ende schliessen
    close(thePipe[READ]);

    // Auf Kind warten
    wait(NULL);

    return EXIT_SUCCESS;
}
