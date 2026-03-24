/*
 * Datei: biggest.c
 * Autor: Student 1
 * Beschreibung: Ermittlung der maximalen Anzahl von Integern die allokiert werden können
 *               Verwendet binäre Suche um effizient das Maximum zu finden
 */

#include <stdio.h>
#include <stdlib.h>

int main() {
    unsigned long untere_grenze, obere_grenze, aktuelle_test_anzahl;
    unsigned long maximale_erfolgreiche_anzahl;
    int* test_speicher_zeiger;
    
    printf("Ermittlung der maximalen Anzahl allozierbarer Integer-Werte...\n");
    printf("Verwende binäre Suche für effiziente Bestimmung.\n\n");
    
    /* Initialisierung der Suchgrenzen */
    untere_grenze = 0;
    obere_grenze = 1000000000UL; /* Starte mit 1 Milliarde als obere Grenze */
    maximale_erfolgreiche_anzahl = 0;
    
    /* Binäre Suche um das Maximum zu finden */
    do {
        /* Berechne die Mitte zwischen unterer und oberer Grenze */
        aktuelle_test_anzahl = untere_grenze + (obere_grenze - untere_grenze) / 2;
        
        printf("Teste Allokierung von %lu Integern (%lu Bytes)...", 
               aktuelle_test_anzahl, aktuelle_test_anzahl * sizeof(int));
        
        /* Versuche den Speicher zu allokieren */
        test_speicher_zeiger = (int*)malloc(aktuelle_test_anzahl * sizeof(int));
        
        if (test_speicher_zeiger != NULL) {
            /* Allokierung erfolgreich */
            printf(" ERFOLGREICH\n");
            maximale_erfolgreiche_anzahl = aktuelle_test_anzahl;
            
            /* Gebe den Speicher sofort wieder frei */
            free(test_speicher_zeiger);
            
            /* Suche in der oberen Hälfte weiter */
            untere_grenze = aktuelle_test_anzahl + 1;
        } else {
            /* Allokierung fehlgeschlagen */
            printf(" FEHLGESCHLAGEN\n");
            
            /* Suche in der unteren Hälfte weiter */
            obere_grenze = aktuelle_test_anzahl - 1;
        }
        
    } while (untere_grenze <= obere_grenze);
    
    printf("\nErgebnis:\n");
    printf("Maximale Anzahl allozierbarer Integer: %lu\n", maximale_erfolgreiche_anzahl);
    printf("Entspricht %lu Bytes (%.2f MB)\n", 
           maximale_erfolgreiche_anzahl * sizeof(int),
           (double)(maximale_erfolgreiche_anzahl * sizeof(int)) / (1024 * 1024));
    
    return 0;
}