// Ü 8.3 - Threads/Sync: Semaphoren-Bug finden und beheben
//
//
// === Aufgabe ===
//
// Der Kollege hat zwei Semaphoren (sem1, sem2) zum Schutz zweier
// geteilter Ressourcen (r1, r2) verwendet, und manchmal "verkeilen"
// sich die Threads. Problem suchen, beheben und benennen.
//
//
// === Das Problem im Originalcode ===
//
// Der Originalcode lockt sem1 einmal vor der Schleife und führt
// danach ein Lock-Handover durch:
//
//     sem_wait(sem1)             // einmal vor der Schleife
//     while (1) {
//         modifiziere r1         // sem1 wird gehalten
//         sem_wait(sem2)         // hält jetzt sem1 UND sem2
//         sem_post(sem1)         // gibt sem1 frei, behält sem2
//         modifiziere r2         // sem2 wird gehalten
//         sem_wait(sem1)         // hält jetzt sem2 UND will sem1
//         sem_post(sem2)         // gibt sem2 frei
//     }
//
// Wenn Thread A nach dem sem_post(sem1) gerade r2 unter sem2 ändert,
// kann Thread B inzwischen sem1 belegen. Jetzt:
//
//   - Thread A hält sem2 und wartet auf sem1.
//   - Thread B hält sem1 und wartet auf sem2.
//
// Beide warten gegenseitig aufeinander. Niemand gibt etwas frei.
//
//
// === Wie nennt man das? ===
//
// Deadlock (Verklemmung), genauer: Circular Wait durch das gleichzeitige
// Halten und Anfordern unterschiedlicher Sperren. Alle vier Coffman-
// Bedingungen sind erfüllt:
//
//   1. Mutual Exclusion    Semaphoren sperren exklusiv.
//   2. Hold and Wait       Thread hält eine Sperre und wartet auf eine zweite.
//   3. No Preemption       Eine gehaltene Semaphore kann nicht weggenommen werden.
//   4. Circular Wait       A wartet auf B, B wartet auf A.
//
//
// === Die Lösung ===
//
// Wir verwenden für jede Ressource ein sauberes lock/unlock-Paar.
// Kein Thread hält gleichzeitig mehr als eine Semaphore. Damit fällt
// "Hold and Wait" weg und der Deadlock kann nicht mehr entstehen.
//
// Zusätzlich wurde die Endlosschleife durch eine feste Iterationszahl
// ersetzt, damit das Programm sauber terminiert. Die Thread-Anzahl
// wurde auf einen handhabbaren Wert reduziert.

#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>

#define NUM_THREADS 10
#define MAX_ITER    100

int shared_resource_1 = 0;
int shared_resource_2 = 0;

sem_t semaphore_1;
sem_t semaphore_2;

void *thread_function(void *arg)
{
    int thread_id = *((int *)arg);

    for (int i = 0; i < MAX_ITER; i++)
    {
        // Kritischer Abschnitt für r1
        sem_wait(&semaphore_1);
        shared_resource_1 += thread_id;
        sem_post(&semaphore_1);

        // Kritischer Abschnitt für r2
        sem_wait(&semaphore_2);
        shared_resource_2 += thread_id * 2;
        sem_post(&semaphore_2);
    }

    printf("Thread %2d hat %d Iterationen abgeschlossen.\n", thread_id, MAX_ITER);
    pthread_exit(NULL);
}

int main(void)
{
    sem_init(&semaphore_1, 0, 1);
    sem_init(&semaphore_2, 0, 1);

    pthread_t threads[NUM_THREADS];
    int       thread_ids[NUM_THREADS];

    for (int i = 0; i < NUM_THREADS; i++)
    {
        thread_ids[i] = i + 1;
        pthread_create(&threads[i], NULL, thread_function, &thread_ids[i]);
    }

    for (int i = 0; i < NUM_THREADS; i++)
    {
        pthread_join(threads[i], NULL);
    }

    printf("\nEndwerte: r1 = %d, r2 = %d\n",
           shared_resource_1, shared_resource_2);

    sem_destroy(&semaphore_1);
    sem_destroy(&semaphore_2);

    return 0;
}
