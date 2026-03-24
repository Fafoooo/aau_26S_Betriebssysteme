/*
 * Datei: endless.c
 * Autor: Student 1
 * Beschreibung: Endlosschleife zur Demonstration von ulimit -t
 *               Zeigt wie das System nach 4 Sekunden das Programm beendet
 */

#include <stdio.h>
#include <time.h>

int main() {
    unsigned long long schleifenzaehler;
    time_t start_zeit, aktuelle_zeit;
    
    printf("Starte Endlosschleife...\n");
    printf("Verwende 'ulimit -t 4' vor dem Ausführen um nach 4 Sekunden zu beenden.\n");
    printf("Ohne ulimit läuft das Programm unendlich weiter.\n\n");
    
    /* Speichere die Startzeit für Statistiken */
    start_zeit = time(NULL);
    schleifenzaehler = 0;
    
    /* Endlosschleife mit do-while */
    do {
        schleifenzaehler++;
        
        /* Gebe alle 10 Millionen Iterationen eine Meldung aus */
        if (schleifenzaehler % 10000000 == 0) {
            aktuelle_zeit = time(NULL);
            printf("Iteration %llu erreicht nach %ld Sekunden\n", 
                   schleifenzaehler, aktuelle_zeit - start_zeit);
        }
        
        /* Diese Schleife läuft theoretisch unendlich weiter */
    } while (1);
    
    /* Dieser Code wird nie erreicht (außer durch Signal-Terminierung) */
    printf("Das Programm wurde beendet.\n");
    return 0;
}