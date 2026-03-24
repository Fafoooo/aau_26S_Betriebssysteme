// Ü 4.4 - Race Conditions mit 20 Kindprozessen
//
// ANTWORTEN:
//
// 1) Terminiert das Programm regulär?
//    Ja, jeder Kindprozess zaehlt seine PID-mal hoch und beendet sich dann.
//    Der Elternprozess wartet auf alle 20 Kinder mit wait().
//
// 2) Warum erreichen wir nicht 380.000?
//    Nach fork() bekommt JEDER Kindprozess eine EIGENE KOPIE des Adressraums
//    (Copy-on-Write). Die globale Variable wird also in jedem Kind separat
//    hochgezaehlt. Jedes Kind sieht nur seinen eigenen Zaehler, nicht die
//    Aenderungen der anderen Kinder.
//    -> Es gibt KEIN Shared Memory zwischen den Prozessen!
//    -> Jeder Kindprozess zaehlt nur bis zu seiner eigenen PID.
//    -> Die Summe kommt nie zusammen weil die Variable nicht geteilt wird.
//
// 3) Wie kann man den Zugriff sichern?
//    a) Shared Memory (mmap mit MAP_SHARED oder shmget/shmat) + Semaphore
//    b) Shared Memory + Mutex (pthread_mutex mit PTHREAD_PROCESS_SHARED)
//    c) Named Semaphores (sem_open)
//    d) File Locking (flock/fcntl)
//    e) Pipes oder Message Queues fuer die Kommunikation

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#define NUM_CHILDREN 20

// Globale Variable - wird aber per fork() kopiert, nicht geteilt!
unsigned long global_count = 0;

int main(void)
{
    printf("[eltern] pid %d startet %d Kinder\n\n", getpid(), NUM_CHILDREN);

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
            pid_t my_pid = getpid();
            printf("[kind] pid %d von [eltern] pid %d\n", my_pid, getppid());

            // So oft inkrementieren wie die eigene PID
            for (long c = 0; c < my_pid; c++)
            {
                global_count++;
            }

            printf("[kind] pid %d zaehlt bis %lu\n\n", my_pid, global_count);
            exit(EXIT_SUCCESS);
        }
    }

    // Elternprozess: auf alle Kinder warten
    for (int i = 0; i < NUM_CHILDREN; i++)
    {
        wait(NULL);
    }

    // global_count hier ist immer noch 0 (Eltern hat nicht gezaehlt)
    printf("[eltern] global_count im Elternprozess: %lu (unveraendert!)\n", global_count);

    return EXIT_SUCCESS;
}
