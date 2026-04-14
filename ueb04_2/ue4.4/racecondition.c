// Übung 4.4 - Race Conditions mit 20 Kindprozessen
//
//
// == Aufgabe ==
//
// Es werden 20 Kindprozesse erstellt. Jeder Kindprozess erhöht eine
// globale Variable so oft wie seine eigene PID. Also z.B. ein Prozess
// mit PID 42 erhöht die Variable 42 Mal. Die globale Variable wird
// außerhalb von main deklariert.
//
//
// == Antworten ==
//
// 1) Terminiert das Programm korrekt?
//    Ja. Alle Kinder zählen und beenden sich mit exit(). Der
//    Elternprozess ruft für jedes Kind wait() auf.
//
//
// 2) Warum wird nicht der erwartete Gesamtwert erreicht?
//    fork() erzeugt eine Kopie des gesamten Adressraums für jedes Kind
//    (Copy-on-Write). Die Variable zaehler existiert somit einmal
//    pro Kindprozess — sie ist nicht zwischen den Prozessen geteilt.
//
//    Jedes Kind zählt nur in seiner eigenen lokalen Kopie hoch und
//    bekommt von den Änderungen der anderen nichts mit. Im
//    Elternprozess bleibt die Variable bei 0, weil dieser nie
//    inkrementiert hat.
//
//    Technisch gesehen ist das also gar keine echte Race Condition,
//    sondern das Problem ist, dass kein Shared Memory verwendet wird.
//
//
// 3) Wie könnte man den Zugriff auf eine geteilte Variable absichern?
//    Man braucht zwei Dinge: tatsächlich geteilten Speicher und
//    Synchronisation.
//
//    Für geteilten Speicher:
//      - mmap() mit MAP_SHARED
//      - shmget()/shmat() (System V Shared Memory)
//
//    Für die Synchronisation:
//      - POSIX Semaphoren (sem_open/sem_wait/sem_post)
//      - pthread_mutex mit PTHREAD_PROCESS_SHARED
//      - Dateisperren via flock()/fcntl()

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

// Globale Variable — jedes Kind bekommt bei fork() eine eigene Kopie davon.
unsigned long zaehler = 0;

int main(void)
{
    int anz_kinder = 20;

    for (int i = 0; i < anz_kinder; i++)
    {
        if (fork() == 0)
        {
            pid_t mein_pid = getpid();
            printf("[Kind] PID %d, Eltern-PID %d\n", mein_pid, getppid());

            for (long j = 0; j < mein_pid; j++)
            {
                zaehler++;
            }

            printf("[Kind] PID %d -> zaehler = %lu\n", mein_pid, zaehler);
            exit(EXIT_SUCCESS);
        }
    }

    for (int i = 0; i < anz_kinder; i++)
    {
        wait(NULL);
    }

    // Im Elternprozess ist zaehler immer noch 0, da er selbst nie
    // inkrementiert hat. Jedes Kind hatte nur seine eigene Kopie.
    printf("[Eltern] zaehler = %lu (nicht verändert)\n", zaehler);

    return EXIT_SUCCESS;
}
