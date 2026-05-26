/**
 * UE Betriebssysteme
 *
 * Threads - Bingo
 */
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <unistd.h>

#define N 3
#define CARDSIZE 10
#define NUMBERS 50
#define NUM_THREADS (N + 2)  // N+2 threads (players + showmaster + monitor)

pthread_t thread[NUM_THREADS];

// player thread
void* player(void* id)
{
    // TODO
}

// showmaster thread
void* showmaster(void* id)
{
    // TODO
}

// monitor thread
void* monitor(void* id)
{
    // TODO
}

int main(int argc, char* argv[])
{
    // seed random number generator
    srand(time(NULL));

    // init barriers
    // TODO

    // create threads
    int  i;
    int* thread_id = (int*) malloc(sizeof(int) * NUM_THREADS);

    // players (id:0 to N-1)
    for (i = 0; i < N; i++)
    {
        thread_id[i] = i;
        pthread_create(&thread[i], NULL, player, (void*) &thread_id[i]);
    }

    // showmaster (id:N)
    thread_id[N] = N;
    pthread_create(&thread[N], NULL, showmaster, (void*) &thread_id[N]);

    // monitor (id:N+1)
    thread_id[N + 1] = N + 1;
    pthread_create(&thread[N + 1], NULL, monitor, (void*) &thread_id[N + 1]);

    // wait for threads to join
    for (i = 0; i < NUM_THREADS; i++)
    {
        pthread_join(thread[i], NULL);
    }

    free(thread_id);

    return EXIT_SUCCESS;
}