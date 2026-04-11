// Ü 4.4 - Race Conditions mit 20 Kindprozessen
//
//
// === Aufgabe ===
//
// 20 Kindprozesse werden erzeugt. Jedes Kind inkrementiert eine globale
// Variable so oft, wie seine eigene PID angibt. Ein Kind mit der PID 3
// würde die globale Variable also 3-mal um 1 erhöhen. Die Variable wird
// außerhalb von main() deklariert.
//
//
// === Antworten zu den Fragen ===
//
// 1) Terminiert das Programm regulär?
//    Ja. Jedes Kind zählt, gibt seinen Wert aus und beendet sich mit
//    exit(). Der Elternprozess wartet mit wait() auf alle 20 Kinder.
//
//
// 2) Warum erreichen wir nicht 380.000?
//    Weil fork() jedem Kind eine eigene Kopie des Adressraums gibt
//    (Copy-on-Write). Die globale Variable ist damit nicht geteilt,
//    sondern jedes Kind hat seine eigene.
//
//    Jedes Kind zählt also nur in seiner eigenen Kopie hoch und sieht
//    die Änderungen der anderen Kinder gar nicht. Die Einzelwerte
//    werden nie zusammengeführt. Der Elternprozess selbst hat ebenfalls
//    nicht inkrementiert, also steht in seinem global_count am Ende
//    sogar nur 0.
//
//    Das ist streng genommen keine klassische Race Condition, sondern
//    fehlendes Shared Memory.
//
//
// 3) Wie kann man den Zugriff auf eine echte gemeinsame Variable sichern?
//    Zwei Dinge sind nötig: echtes Shared Memory und eine Synchronisation.
//
//    Für Shared Memory:
//      - mmap mit MAP_SHARED
//      - shmget / shmat (System V)
//
//    Für die Synchronisation der Zugriffe:
//      - POSIX Semaphoren (sem_open)
//      - pthread_mutex mit PTHREAD_PROCESS_SHARED
//      - File Locking (flock / fcntl) als einfachere Alternative

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

// Globale Variable - wird bei fork() in jedes Kind kopiert, nicht geteilt.
unsigned long global_count = 0;

int main(void)
{
    const int num_children = 20;

    for (int i = 0; i < num_children; i++)
    {
        if (fork() == 0)
        {
            pid_t my_pid = getpid();
            printf("[kind] pid %d von [eltern] pid %d\n", my_pid, getppid());

            for (long c = 0; c < my_pid; c++)
            {
                global_count++;
            }

            printf("[kind] pid %d zählt bis %lu\n", my_pid, global_count);
            exit(EXIT_SUCCESS);
        }
    }

    // Auf alle Kinder warten
    for (int i = 0; i < num_children; i++)
    {
        wait(NULL);
    }

    // global_count ist hier immer noch 0, weil der Elternprozess selbst
    // nicht inkrementiert hat. Das ist der Beweis, dass jedes Kind seine
    // eigene Kopie hatte.
    printf("[eltern] global_count im Elternprozess: %lu (unverändert!)\n",
           global_count);

    return EXIT_SUCCESS;
}
