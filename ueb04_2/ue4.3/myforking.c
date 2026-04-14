// Übung 4.3 - Fünf Kindprozesse erzeugen und zählen
//
//
// == Aufgabenstellung ==
//
// Ein Elternprozess erstellt 5 Kindprozesse. Jedes Kind soll:
//   - seine PID sowie die PID des Elternprozesses ausgeben
//   - von 1 bis zu seiner eigenen PID hochzählen
//   - sich anschließend beenden
//
// Der Elternprozess wartet danach auf alle Kinder.
//
//
// == Beobachtung zur Reihenfolge ==
//
// Die Kindprozesse laufen parallel. Der Scheduler bestimmt, wann
// welcher Prozess drankommt. Ein Kindprozess mit einer kleineren PID
// kann trotzdem erst später starten, weil der Scheduler einem anderen
// Prozess zuerst CPU-Zeit zuteilt. Die Ausgaben sind daher nicht
// zwangsläufig sortiert.

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

int main(void)
{
    int kinder = 5;

    for (int i = 0; i < kinder; i++)
    {
        if (fork() == 0)
        {
            pid_t meine_pid = getpid();
            printf("[Kind %d] PID=%d, Eltern-PID=%d\n", i, meine_pid, getppid());

            for (long z = 1; z <= meine_pid; z++)
            {
                printf("[Kind %d] PID=%d zaehlt: %ld\n", i, meine_pid, z);
            }

            printf("[Kind %d] PID=%d fertig (gezählt bis %d)\n", i, meine_pid, meine_pid);
            exit(EXIT_SUCCESS);
        }
    }

    for (int i = 0; i < kinder; i++)
    {
        wait(NULL);
    }

    return EXIT_SUCCESS;
}
