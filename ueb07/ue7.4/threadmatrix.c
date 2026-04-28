// Ü 7.4 - Spaltensummen einer NxN-Matrix mit pthreads
//
// Pendant zu forkmatrix.c. Threads teilen sich den Speicher,
// also kein IPC nötig - jeder Thread schreibt sein Ergebnis direkt
// in sums[col].
//
// Laufzeit messen mit:  time ./threadmatrix
//
// Kompilieren:  gcc -O2 -pthread -o threadmatrix threadmatrix.c

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>

#define N 100

static int matrix[N][N];
static long sums[N];

void *column_sum(void *arg)
{
    int col = *(int *)arg;
    long sum = 0;
    for (int row = 0; row < N; row++)
        sum += matrix[row][col];
    sums[col] = sum;
    return NULL;
}

int main(void)
{
    srand((unsigned)time(NULL));
    for (int i = 0; i < N; i++)
        for (int j = 0; j < N; j++)
            matrix[i][j] = rand() % 100;

    pthread_t threads[N];
    int cols[N];   // pro Thread eigener Slot, sonst Race auf &i

    for (int i = 0; i < N; i++) {
        cols[i] = i;
        pthread_create(&threads[i], NULL, column_sum, &cols[i]);
    }
    for (int i = 0; i < N; i++)
        pthread_join(threads[i], NULL);

    for (int i = 0; i < N; i++)
        printf("col %3d sum = %ld\n", i, sums[i]);

    return EXIT_SUCCESS;
}
