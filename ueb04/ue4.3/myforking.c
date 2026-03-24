// Ü 4.3 - 5 Kindprozesse erstellen, jeder zaehlt bis zu seiner PID
//
// Warum sind manche Prozesse schon fertig bevor andere anfangen?
// -> Jeder Kindprozess laeuft unabhaengig. Das OS entscheidet per Scheduler
//    welcher Prozess wann CPU-Zeit bekommt. Es gibt keine garantierte Reihenfolge.
//    Ein Kind mit niedrigerer PID koennte trotzdem spaeter anfangen weil das OS
//    einem anderen Prozess zuerst die CPU gibt.

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#define NUM_CHILDREN 5

int main(void)
{
    for (int i = 0; i < NUM_CHILDREN; i++)
    {
        pid_t pid = fork();

        if (pid < 0)
        {
            perror("fork");
            exit(EXIT_FAILURE);
        }
        else if (pid == 0)
        {
            // Kindprozess
            pid_t my_pid    = getpid();
            pid_t parent_id = getppid();

            printf("[kind] pid %d von [eltern] pid %d\n", my_pid, parent_id);

            // Von (pid - 7) bis pid zaehlen (sonst waere die Ausgabe riesig)
            // Die Angabe sagt "von 1 bis PID", wir zeigen nur die letzten paar
            long start = (my_pid > 7) ? my_pid - 7 : 1;
            for (long c = start; c <= my_pid; c++)
            {
                printf("[kind] pid %d Zaehler: %ld\n", my_pid, c);
            }

            printf("\n[kind] pid %d zaehlt bis %d\n\n", my_pid, my_pid);
            exit(EXIT_SUCCESS);
        }
        // Elternprozess: naechstes Kind erzeugen
    }

    // Elternprozess: auf alle Kinder warten
    for (int i = 0; i < NUM_CHILDREN; i++)
    {
        wait(NULL);
    }

    return EXIT_SUCCESS;
}
