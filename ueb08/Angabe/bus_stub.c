#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>
#include <time.h>
#include "visualization.h"

Passenger passengers[N];

// TODO (Teil 1): Mutex und Condition Variables deklarieren
// (Tipp: Sie brauchen etwas, worauf die Passagiere warten können, wenn der Bus fährt)

// TODO (Teil 2): Globale Zähler für aktuelle Passagiere im Bus und belegte Sitzplätze deklarieren


void* passenger_thread(void* arg) 
{
    Passenger *p = (Passenger *)arg;
	
    // TODO (Teil 1): Mutex locken
		
    // 1. Warten bis der Bus an der EIGENEN Haltestelle angekommen ist.
    // (Tipp: pthread_cond_wait in einer while-Schleife nutzen, die p->status und die Busposition prüft)

    // TODO (Teil 2): Hier zusätzlich prüfen, ob der Bus voll ist. Wenn ja, weiter warten!

    // 2. Einsteigen 
    // TODO (Teil 1): Status vorerst einfach auf BUS_STANDING setzen.
    // TODO (Teil 2): Prüfen, ob ein Sitzplatz frei ist. Wenn ja: BUS_SEATED und Sitzplatz-Zähler erhöhen.
    //                Wenn nein: BUS_STANDING. Bus-Kapazitäts-Zähler in beiden Fällen erhöhen.

    // Mutex unlocken, damit andere einsteigen können
    
    // 3. Schlafen (Reisezeit simulieren)
    passenger_sleep();

    // TODO (Teil 1): Mutex wieder locken
    
    // 4. Warten bis der Bus wieder an EINER Haltestelle ist (egal welche, Hauptsache er hält an)

    // 5. Aussteigen
    p->status = DONE;
    // TODO (Teil 2): Bus-Kapazitäts-Zähler und ggf. Sitzplatz-Zähler wieder verringern.

    // TODO (Teil 1): Mutex unlocken
	
    // Prüfen, ob das Programm beendet werden kann
    done = check_finish(passengers);
    pthread_exit(NULL);
}

void init_passengers()
{
    // Array für die Thread-IDs
    // TODO (Teil 1): pthread_t Array für N Passagiere anlegen

    for (int i = 0; i < N; i++) 
    {
        passengers[i].id = 'A' + (i % 26); // Modulo verhindert seltsame Zeichen bei N > 26
        int station = rand() % 2;  
        if (station == 0) 
        {
            passengers[i].status = AT_A;
        } 
        else 
        {
            passengers[i].status = AT_B;
        }
		
        // TODO (Teil 1): Einen Thread pro Passagier starten (pthread_create)
        // Übergeben Sie &passengers[i] als Argument an den Thread.
    }

    // (Die Threads werden später in main() mit pthread_join eingesammelt)
}

void bus_arrived_in_station()
{
    // TODO (Teil 1): Passagiere aufwecken! Der Bus ist angekommen.
    // (Tipp: Mutex locken, pthread_cond_broadcast aufrufen, Mutex unlocken)
}

int main() 
{
    srand(time(NULL));

    // TODO (Teil 1): Mutex und Condition Variables initialisieren (pthread_mutex_init, etc.)

    init_visualization();
    init_passengers();
    
    // Startet die ncurses Schleife. Blockiert, bis 'done' == 1 ist.
    simulate_bus_movement(passengers);
	
    // TODO (Teil 1): Aufräumen
    // 1. Alle Passagier-Threads einsammeln (pthread_join)
    // 2. Mutex und Condition Variables zerstören (pthread_mutex_destroy, etc.)
	
    close_visualization();
    return 0;
}