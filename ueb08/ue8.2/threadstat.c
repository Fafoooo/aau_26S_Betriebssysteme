// Ü 8.2 - Threads: Statistik mit drei Worker-Threads
//
//
// === Aufgabe ===
//
// Für eine feste Zahlenliste werden drei statistische Werte parallel
// in eigenen Threads berechnet:
//
//   - Thread 1: Durchschnitt (AVG)
//   - Thread 2: Minimum (MIN)
//   - Thread 3: Maximum (MAX)
//
// Jeder Thread bekommt einen Pointer auf eine gemeinsame stats_t-Struktur
// und schreibt sein Ergebnis in das eigene Feld. Weil jeder Thread nur
// sein eigenes Feld schreibt und nur lesend auf das Array zugreift,
// gibt es keine Race Condition und es braucht kein Mutex.
//
// main() startet die drei Threads, wartet mit pthread_join auf alle
// drei und gibt am Ende die Werte aus. Damit terminiert das Programm
// sauber.
//
//
// === Beispielausgabe ===
//
// Input:  {22, 28, 3, 65, 43, 22, 12, 99, 102, 38, 21}
// Output: AVG: 41.36
//         MIN: 3
//         MAX: 102

#include <stdio.h>
#include <pthread.h>

#define ARRAY_SIZE 11

static int numbers[ARRAY_SIZE] = {22, 28, 3, 65, 43, 22, 12, 99, 102, 38, 21};

typedef struct
{
    int    *data;
    int     size;
    double  avg;
    int     min;
    int     max;
} stats_t;

void *compute_avg(void *arg)
{
    stats_t *s = (stats_t *)arg;
    long sum = 0;
    for (int i = 0; i < s->size; i++)
        sum += s->data[i];
    s->avg = (double)sum / s->size;
    return NULL;
}

void *compute_min(void *arg)
{
    stats_t *s = (stats_t *)arg;
    int min = s->data[0];
    for (int i = 1; i < s->size; i++)
        if (s->data[i] < min)
            min = s->data[i];
    s->min = min;
    return NULL;
}

void *compute_max(void *arg)
{
    stats_t *s = (stats_t *)arg;
    int max = s->data[0];
    for (int i = 1; i < s->size; i++)
        if (s->data[i] > max)
            max = s->data[i];
    s->max = max;
    return NULL;
}

int main(void)
{
    stats_t stats = {
        .data = numbers,
        .size = ARRAY_SIZE,
    };

    pthread_t t_avg, t_min, t_max;

    pthread_create(&t_avg, NULL, compute_avg, &stats);
    pthread_create(&t_min, NULL, compute_min, &stats);
    pthread_create(&t_max, NULL, compute_max, &stats);

    pthread_join(t_avg, NULL);
    pthread_join(t_min, NULL);
    pthread_join(t_max, NULL);

    printf("AVG: %.2f\n", stats.avg);
    printf("MIN: %d\n", stats.min);
    printf("MAX: %d\n", stats.max);

    return 0;
}
