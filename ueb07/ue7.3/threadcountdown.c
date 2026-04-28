// Ü 7.3 - Countdown mit POSIX Threads
//
// Aufruf:  ./threadcountdown 3 5 4
//   -> 3 Threads zählen parallel von 3, 5 und 4 bis 0 herunter
//      (1 Sekunde pro Schritt), jeder gibt dabei seine Thread-ID mit aus.
//
// Falle:  jedem Thread einen EIGENEN int-Slot übergeben (values[i]),
//         nicht &i - sonst sehen alle Threads dieselbe Adresse,
//         deren Wert sich währenddessen weiter erhöht (Race).
//
// Kompilieren:  gcc -pthread -o threadcountdown threadcountdown.c

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

void *countdown(void *arg)
{
    int start = *(int *)arg;
    pthread_t self = pthread_self();

    printf("Thread %lu: will count from %d to 0\n", (unsigned long)self, start);
    for (int i = start; i >= 0; i--) {
        printf("Thread %lu:   %d\n", (unsigned long)self, i);
        if (i > 0) sleep(1);
    }
    printf("Thread %lu: finished...\n", (unsigned long)self);
    return NULL;
}

int main(int argc, char *argv[])
{
    if (argc < 2) {
        fprintf(stderr, "Usage: %s p1 p2 ... pn\n", argv[0]);
        return EXIT_FAILURE;
    }

    int n = argc - 1;
    pthread_t threads[n];
    int values[n];

    for (int i = 0; i < n; i++) {
        values[i] = atoi(argv[i + 1]);
        pthread_create(&threads[i], NULL, countdown, &values[i]);
    }
    for (int i = 0; i < n; i++) {
        pthread_join(threads[i], NULL);
    }

    return EXIT_SUCCESS;
}
