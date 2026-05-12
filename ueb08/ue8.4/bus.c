// Ü 8.4 + 8.5 - Bus-Simulation mit Threads
//
//
// === Aufgabe ===
//
// Ein Bus pendelt zwischen den Haltestellen A und B. Jeder Passagier
// ist ein eigener Thread und durchläuft folgende Schritte:
//
//   1. An seiner Haltestelle warten, bis der Bus ankommt.
//   2. Einsteigen. Wenn ein Sitzplatz frei ist: BUS_SEATED, sonst
//      BUS_STANDING. Wenn der Bus voll ist (M Passagiere): warten.
//   3. Während der Fahrt schlafen (passenger_sleep).
//   4. An der nächsten Haltestelle aussteigen (Status DONE).
//
// Konstanten aus visualization.h:
//   - N = 26   Anzahl der Passagiere
//   - M = 9    Gesamtkapazität des Busses
//   - X = 4    Sitzplätze
//
//
// === Synchronisation ===
//
// Ein Mutex schützt alle geteilten Variablen (Passagier-Status,
// Zähler für Bus-Auslastung und Sitzplätze).
//
// Eine Condition Variable signalisiert "der Bus-Zustand hat sich
// geändert". Sie wird signalisiert
//   - vom Bus, wenn er an einer Haltestelle ankommt (bus_arrived_in_station)
//   - von Passagieren, die aussteigen und damit einen Platz freigeben
//
// Wartende Passagiere prüfen nach jedem Aufwachen ihre Bedingung neu
// (klassisches "while + pthread_cond_wait"-Muster).
//
//
// === Kompilieren und starten ===
//
//   gcc bus.c visualization.c -o bus -lncurses -pthread
//   ./bus

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>
#include <time.h>
#include "visualization.h"

Passenger passengers[N];
pthread_t threads[N];

// Mutex und Condition Variable mit statischen Initialisierern.
static pthread_mutex_t mu       = PTHREAD_MUTEX_INITIALIZER;
static pthread_cond_t  cv_state = PTHREAD_COND_INITIALIZER;

// Aktuelle Auslastung des Busses.
static int bus_count  = 0;
static int seat_count = 0;

void *passenger_thread(void *arg)
{
    Passenger *p = (Passenger *)arg;

    pthread_mutex_lock(&mu);

    // 1. Warten, bis der Bus an der eigenen Haltestelle hält UND noch Platz hat.
    while (1)
    {
        int at_my_station = (p->status == AT_A && BUS_AT_A) ||
                            (p->status == AT_B && BUS_AT_B);
        if (at_my_station && bus_count < M)
            break;
        pthread_cond_wait(&cv_state, &mu);
    }

    // 2. Einsteigen: Sitzplatz nehmen wenn frei, sonst stehen.
    if (seat_count < X)
    {
        p->status = BUS_SEATED;
        seat_count++;
    }
    else
    {
        p->status = BUS_STANDING;
    }
    bus_count++;

    pthread_mutex_unlock(&mu);

    // 3. Reisezeit simulieren (außerhalb des Locks, damit andere weiterarbeiten).
    passenger_sleep();

    pthread_mutex_lock(&mu);

    // 4. Warten, bis der Bus an irgendeiner Haltestelle hält.
    while (!(BUS_AT_A || BUS_AT_B))
    {
        pthread_cond_wait(&cv_state, &mu);
    }

    // 5. Aussteigen: Status auf DONE, Zähler entsprechend zurücksetzen.
    int was_seated = (p->status == BUS_SEATED);
    p->status = DONE;
    bus_count--;
    if (was_seated)
        seat_count--;

    pthread_mutex_unlock(&mu);

    // Wartende Passagiere wecken, weil jetzt wieder Platz frei ist.
    pthread_cond_broadcast(&cv_state);

    // Prüfen, ob alle Passagiere fertig sind (setzt 'done' = 1, wenn ja).
    done = check_finish(passengers);

    pthread_exit(NULL);
}

void init_passengers()
{
    for (int i = 0; i < N; i++)
    {
        passengers[i].id     = 'A' + (i % 26);
        passengers[i].status = (rand() % 2 == 0) ? AT_A : AT_B;

        pthread_create(&threads[i], NULL, passenger_thread, &passengers[i]);
    }
}

void bus_arrived_in_station()
{
    // Alle wartenden Passagiere wecken, damit AT_A/AT_B-Passagiere
    // bei ihrer Haltestelle einsteigen können.
    pthread_mutex_lock(&mu);
    pthread_cond_broadcast(&cv_state);
    pthread_mutex_unlock(&mu);
}

int main(void)
{
    srand(time(NULL));

    init_visualization();
    init_passengers();

    // Startet die ncurses-Schleife. Blockiert, bis 'done' == 1 ist.
    simulate_bus_movement(passengers);

    // Alle Passagier-Threads einsammeln.
    for (int i = 0; i < N; i++)
    {
        pthread_join(threads[i], NULL);
    }

    pthread_mutex_destroy(&mu);
    pthread_cond_destroy(&cv_state);

    close_visualization();
    return 0;
}
