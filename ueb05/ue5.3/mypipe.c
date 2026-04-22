// Ü 5.3 - Interprozesskommunikation: Pipes
//
//
// === Aufgabe ===
//
// Ein Elternprozess und ein Kindprozess kommunizieren über eine Pipe.
// Das Kind erzeugt eine variable Anzahl zufälliger int-Werte und schickt
// sie an den Elternprozess. Der Elternprozess liest die Werte und gibt
// am Ende den Durchschnitt aus.
//
//
// === Wie weiß der Elternprozess, wie viele Werte kommen? ===
//
// Das Kind schickt zuerst einen einzelnen int-Wert: die Anzahl der
// folgenden Werte. Der Elternprozess liest diese Zahl zuerst und weiß
// dann, wie oft er lesen muss.
//
// Alternativen wären:
//   - Ein Sentinel-Wert (zum Beispiel -1) am Ende des Stroms
//   - Schließen der Pipe auf der Schreibseite; read() gibt dann 0 zurück

#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>

#define READ  0
#define WRITE 1

int main(void)
{
    int   thePipe[2];
    pid_t pid;

    if (pipe(thePipe) == -1)
    {
        perror("pipe");
        return EXIT_FAILURE;
    }

    pid = fork();
    if (pid < 0)
    {
        perror("fork");
        return EXIT_FAILURE;
    }

    if (pid == 0)
    {
        // === Kindprozess ===
        close(thePipe[READ]);  // Kind schreibt nur

        srand(time(NULL) ^ getpid());
        int count = 3 + rand() % 8;  // zufällige Anzahl zwischen 3 und 10

        // Zuerst die Anzahl senden, dann die Werte selbst.
        write(thePipe[WRITE], &count, sizeof(int));
        printf("[Kind PID %d] Sende %d Werte:\n", getpid(), count);

        for (int i = 0; i < count; i++)
        {
            int value = rand() % 100;
            printf("[Kind] Sende Wert %d: %d\n", i + 1, value);
            write(thePipe[WRITE], &value, sizeof(int));
            sleep(rand() % 2);  // kleine Pause für bessere Lesbarkeit
        }

        close(thePipe[WRITE]);
        exit(EXIT_SUCCESS);
    }

    // === Elternprozess ===
    close(thePipe[WRITE]);  // Eltern liest nur

    int count;
    read(thePipe[READ], &count, sizeof(int));
    printf("[Eltern PID %d] Erwarte %d Werte:\n", getpid(), count);

    long sum = 0;
    int  value;
    for (int i = 0; i < count; i++)
    {
        read(thePipe[READ], &value, sizeof(int));
        sum += value;
        printf("[Eltern] Empfange Wert %d: %d\n", i + 1, value);
    }

    double avg = (double)sum / count;
    printf("\n[Eltern] Summe: %ld, Anzahl: %d, Durchschnitt: %.2f\n",
           sum, count, avg);

    close(thePipe[READ]);
    wait(NULL);

    return EXIT_SUCCESS;
}
