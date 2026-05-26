/**
 * UE Betriebssysteme - Übungsblatt 10
 *
 * Ü 10.5 - Threads: Barrieren-Bingo
 *
 *
 * === Spielablauf ===
 *
 * N Spielende haben je eine Karte mit 10 verschiedenen Zahlen (1-50).
 * Ein Showmaster zieht maximal einmal pro Sekunde eine noch nicht
 * gezogene Zahl. Pro Runde laufen drei Phasen ab:
 *
 *   1. Der Showmaster zieht eine Zahl.
 *   2. Alle Spielenden prüfen ihre Karte und streichen die Zahl (-> 0).
 *   3. Der Monitor-Thread gibt den aktuellen Spielstand aus.
 *
 * Synchronisiert wird mit einer pthread_barrier: alle N+2 Threads
 * (Spielende + Showmaster + Monitor) treffen sich an drei Barrieren
 * pro Runde. Dadurch zieht der Showmaster erst wieder, wenn alle
 * geprüft haben UND der Monitor ausgegeben hat.
 *
 *
 * === Spielende ===
 *
 * Hat ein Spieler alle Zahlen gestrichen, hat er gewonnen. Er beendet
 * mit pthread_cancel alle anderen Threads und beendet sich selbst.
 * Die anderen Threads haben mit pthread_testcancel Cancellation-Points.
 * Zusätzlich sorgt die Flag game_over dafür, dass nach der Gewinn-Runde
 * niemand erneut in eine Barriere läuft (kein Deadlock).
 *
 *
 * === Kompilieren ===
 *
 *   gcc bingo.c -o bingo -pthread
 *   ./bingo
 */
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

#define N 3
#define CARDSIZE 10
#define NUMBERS 50
#define NUM_THREADS (N + 2)  // N Spieler + Showmaster + Monitor

pthread_t thread[NUM_THREADS];
pthread_barrier_t barrier;

int cards[N][CARDSIZE];          // Karten der Spielenden
int current_number = 0;          // aktuell gezogene Zahl
int used[NUMBERS + 1] = {0};     // schon gezogene Zahlen
volatile int winner_id = -1;     // Gewinner (oder -1)
volatile int game_over = 0;      // Spiel beendet?

// Spieler füllt seine Karte mit 10 verschiedenen Zahlen aus 1..50.
// rand_r mit eigenem Seed -> thread-safe, kein gemeinsamer RNG-Zustand.
static void fill_card(int id, unsigned int *seed)
{
    int count = 0;
    while (count < CARDSIZE)
    {
        int num = 1 + rand_r(seed) % NUMBERS;
        int duplicate = 0;
        for (int i = 0; i < count; i++)
        {
            if (cards[id][i] == num)
            {
                duplicate = 1;
                break;
            }
        }
        if (!duplicate)
            cards[id][count++] = num;
    }
}

// player thread
void* player(void* arg)
{
    int id = *(int*) arg;
    unsigned int seed = (unsigned int)(time(NULL) ^ (id * 2654435761u));

    // Karte zu Spielbeginn füllen.
    fill_card(id, &seed);

    // Warten, bis alle Karten gefüllt sind und der Showmaster bereit ist.
    pthread_barrier_wait(&barrier);

    while (1)
    {
        // B1: Showmaster hat gezogen, Zahl steht in current_number.
        pthread_barrier_wait(&barrier);

        // Gezogene Zahl prüfen und ggf. streichen.
        for (int i = 0; i < CARDSIZE; i++)
        {
            if (cards[id][i] == current_number)
                cards[id][i] = 0;
        }

        // Habe ich alle Zahlen gestrichen?
        int won = 1;
        for (int i = 0; i < CARDSIZE; i++)
        {
            if (cards[id][i] != 0)
            {
                won = 0;
                break;
            }
        }
        if (won)
        {
            winner_id = id;
            game_over = 1;
        }

        // B2: alle Spielenden haben geprüft.
        pthread_barrier_wait(&barrier);

        // B3: der Monitor hat den Spielstand ausgegeben.
        pthread_barrier_wait(&barrier);

        if (game_over)
        {
            if (winner_id == id)
            {
                // Gewonnen: alle anderen Threads beenden.
                for (int t = 0; t < NUM_THREADS; t++)
                {
                    if (t != id)
                        pthread_cancel(thread[t]);
                }
                printf("\n>>> Spieler %d hat BINGO und gewinnt! <<<\n", id);
            }
            pthread_testcancel();   // falls schon gecancelt: hier beenden
            break;                  // sonst sauberer Ausstieg
        }

        pthread_testcancel();       // Cancellation-Point im laufenden Spiel
    }

    return NULL;
}

// showmaster thread
void* showmaster(void* arg)
{
    (void) arg;
    unsigned int seed = (unsigned int)(time(NULL) ^ 0x9e3779b9u);

    // Auf gefüllte Karten warten.
    pthread_barrier_wait(&barrier);

    while (1)
    {
        // Eine noch nicht gezogene Zahl auswählen.
        int num;
        do {
            num = 1 + rand_r(&seed) % NUMBERS;
        } while (used[num]);
        used[num] = 1;
        current_number = num;

        printf("\nShowmaster zieht: %d\n", num);

        pthread_barrier_wait(&barrier);   // B1: Spielende dürfen prüfen
        pthread_barrier_wait(&barrier);   // B2: Spielende fertig
        pthread_barrier_wait(&barrier);   // B3: Monitor fertig

        pthread_testcancel();             // durch Gewinner gecancelt?
        if (game_over)
            break;

        sleep(1);                         // maximal eine Ziehung pro Sekunde
    }

    return NULL;
}

// monitor thread
void* monitor(void* arg)
{
    (void) arg;

    // Auf gefüllte Karten warten.
    pthread_barrier_wait(&barrier);

    while (1)
    {
        pthread_barrier_wait(&barrier);   // B1: Showmaster hat gezogen
        pthread_barrier_wait(&barrier);   // B2: Spielende haben geprüft

        // Aktuellen Spielstand ausgeben (nur noch offene Zahlen).
        printf("--- Spielstand ---\n");
        for (int p = 0; p < N; p++)
        {
            printf("Spieler %d:", p);
            for (int i = 0; i < CARDSIZE; i++)
            {
                if (cards[p][i] != 0)
                    printf(" %2d", cards[p][i]);
            }
            printf("\n");
        }

        pthread_barrier_wait(&barrier);   // B3: Ausgabe fertig

        pthread_testcancel();             // durch Gewinner gecancelt?
        if (game_over)
            break;
    }

    return NULL;
}

int main(int argc, char* argv[])
{
    (void) argc;
    (void) argv;

    // seed random number generator
    srand(time(NULL));

    // init barrier für alle N+2 Threads
    pthread_barrier_init(&barrier, NULL, NUM_THREADS);

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

    pthread_barrier_destroy(&barrier);
    free(thread_id);

    return EXIT_SUCCESS;
}
