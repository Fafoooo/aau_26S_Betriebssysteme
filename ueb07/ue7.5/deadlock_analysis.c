// Ü 7.5 - Deadlock-Analyse: Fix durch konsistente Sperrreihenfolge
//
// Ausgangscode siehe ../Angabe/7.5_deadlock_analysis.c. Dort werden die
// Resourcen in ZUFÄLLIGER Reihenfolge gesperrt (shuffle), was zu
// Deadlocks führt.
//
// Fix hier: Resourcen werden AUFSTEIGEND nach Index sortiert gesperrt
// (Lock Ordering). Damit kann kein Circular Wait mehr entstehen.
// Detail-Erklärung in analysis.txt.
//
// Kompilieren:  gcc -pthread -o deadlock_analysis deadlock_analysis.c

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

pthread_mutex_t res_mutex[4];

typedef struct {
    int thread_id;
    int num_needs;
    int needs[3];
} thread_arg_t;

// Insertion-Sort, aufsteigend (für maximal 3 Werte mehr als ausreichend)
static void sort_ascending(int *arr, int n)
{
    for (int i = 1; i < n; i++) {
        int key = arr[i];
        int j = i - 1;
        while (j >= 0 && arr[j] > key) {
            arr[j + 1] = arr[j];
            j--;
        }
        arr[j + 1] = key;
    }
}

void *thread_func(void *arg)
{
    thread_arg_t *t = arg;
    int order[3];
    for (int i = 0; i < t->num_needs; i++)
        order[i] = t->needs[i];

    sort_ascending(order, t->num_needs);   // <- der eigentliche Fix

    printf("T%d attempting locks in order:", t->thread_id);
    for (int i = 0; i < t->num_needs; i++)
        printf(" R%d", order[i] + 1);
    printf("\n");

    for (int i = 0; i < t->num_needs; i++) {
        pthread_mutex_lock(&res_mutex[order[i]]);
        printf("T%d locked R%d\n", t->thread_id, order[i] + 1);
        usleep(100000);  // 100 ms Zugriff simulieren
    }

    printf("T%d has all resources, doing work...\n", t->thread_id);
    usleep(100000);

    for (int i = 0; i < t->num_needs; i++) {
        pthread_mutex_unlock(&res_mutex[order[i]]);
        printf("T%d unlocked R%d\n", t->thread_id, order[i] + 1);
    }
    return NULL;
}

int main(void)
{
    for (int i = 0; i < 4; i++)
        pthread_mutex_init(&res_mutex[i], NULL);

    thread_arg_t args[4] = {
        {1, 3, {0, 1, 2}},  // T1: R1, R2, R3
        {2, 2, {2, 3}},     // T2: R3, R4
        {3, 2, {0, 3}},     // T3: R1, R4
        {4, 2, {0, 3}}      // T4: R1, R4
    };

    pthread_t th[4];
    for (int i = 0; i < 4; i++)
        pthread_create(&th[i], NULL, thread_func, &args[i]);

    for (int i = 0; i < 4; i++)
        pthread_join(th[i], NULL);

    for (int i = 0; i < 4; i++)
        pthread_mutex_destroy(&res_mutex[i]);

    return EXIT_SUCCESS;
}
