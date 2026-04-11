// Ü 4.3 - Prozesse: 5 Kinder erzeugen und zählen lassen
//
//
// === Aufgabe ===
//
// Der Elternprozess erzeugt 5 Kindprozesse. Jedes Kind:
//   - gibt seine eigene PID und die des Elternprozesses aus
//   - zählt von 1 bis zu seiner eigenen PID
//   - beendet sich danach
//
// Der Elternprozess wartet mit wait() auf alle Kinder.
//
//
// === Warum sind manche Prozesse schon fertig, bevor andere anfangen? ===
//
// Jedes Kind läuft als eigenständiger Prozess parallel zu den anderen.
// Der Scheduler des Betriebssystems entscheidet, welcher Prozess wann
// CPU-Zeit bekommt. Es gibt keine garantierte Reihenfolge.
//
// Ein Kind mit niedrigerer PID kann trotzdem später mit dem Zählen
// anfangen, weil das OS einem anderen Prozess zuerst die CPU gibt.
// Umgekehrt kann ein spät erzeugtes Kind bereits fertig sein, bevor
// ein früher erzeugtes überhaupt losgelaufen ist.

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

int main(void)
{
    const int num_children = 5;

    for (int i = 0; i < num_children; i++)
    {
        if (fork() == 0)
        {
            pid_t my_pid = getpid();
            printf("[kind] pid %d von [eltern] pid %d\n", my_pid, getppid());

            for (long c = 1; c <= my_pid; c++)
            {
                printf("[kind] pid %d Zähler: %ld\n", my_pid, c);
            }

            printf("[kind] pid %d zählt bis %d\n", my_pid, my_pid);
            exit(EXIT_SUCCESS);
        }
    }

    // Auf alle Kinder warten
    for (int i = 0; i < num_children; i++)
    {
        wait(NULL);
    }

    return EXIT_SUCCESS;
}
