#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>

#define NUM_COUNT 10000
#define MIN_RANGE 1
#define MAX_RANGE 10
#define NUM_THREADS 10

// Globale Variablen
int numbers[NUM_COUNT];
int partial_sums[NUM_THREADS] = {0};
pthread_mutex_t mutex;

// Funktion, die von Threads ausgeführt wird
void *thread_sum(void *arg) {
    int thread_id = *((int *)arg);
    int start_index = thread_id * (NUM_COUNT / NUM_THREADS);
    int end_index = (thread_id + 1) * (NUM_COUNT / NUM_THREADS);

    for (int i = start_index; i < end_index; i++) {
        pthread_mutex_lock(&mutex);
        partial_sums[thread_id] += numbers[i];
        pthread_mutex_unlock(&mutex);
    }

    pthread_exit(NULL);
}

int main() {
    pthread_t threads[NUM_THREADS];
    int thread_ids[NUM_THREADS];
    clock_t start, end;
    double cpu_time_used;

    // Initialisiere den Zufallszahlengenerator
    srand(time(NULL));

    // Generiere Zufallszahlen
    printf("Generiere Zufallszahlen...%d \n", NUM_COUNT);
    for (int i = 0; i < NUM_COUNT; i++) {
        numbers[i] = (rand() % (MAX_RANGE - MIN_RANGE + 1)) + MIN_RANGE;
    }

    // Initialisiere Mutex
    pthread_mutex_init(&mutex, NULL);

    // Erstelle Threads
    printf("Erstelle Threads...\n");
    for (int i = 0; i < NUM_THREADS; i++) {
        thread_ids[i] = i;
        pthread_create(&threads[i], NULL, thread_sum, &thread_ids[i]);
    }

    // Starte Zeitmessung
    start = clock();

    // Warte auf Threads
    printf("Warte auf Threads...\n");
    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_join(threads[i], NULL);
    }

    // Stoppe Zeitmessung
    end = clock();
    cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;

    // Zerstöre Mutex
    pthread_mutex_destroy(&mutex);

    // Berechne Gesamtsumme
    int total_sum = 0;
    for (int i = 0; i < NUM_THREADS; i++) {
        total_sum += partial_sums[i];
    }

    // Gebe Summe und benötigte Zeit aus
    printf("Die Summe der Zufallszahlen beträgt: %d\n", total_sum);
    printf("Benötigte Zeit zur Berechnung der Teilsummen: %f Sekunden\n", cpu_time_used);

    return 0;
}

