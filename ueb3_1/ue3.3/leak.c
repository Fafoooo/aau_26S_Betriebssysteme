/*
 * Datei: leak.c
 * Autor: Student 1
 * Beschreibung: Demonstration eines Memory Leaks durch malloc ohne free
 *               Das Programm allokiert kontinuierlich Speicher bis zum Crash
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main() {
    unsigned int allokierungs_zaehler;
    char* speicher_zeiger;
    const unsigned int speicher_block_groesse = 1024 * 1024; /* 1 MB pro Allokierung */
    
    printf("Memory Leak Demonstration gestartet...\n");
    printf("Allokiere kontinuierlich 1MB Blöcke ohne free() aufzurufen.\n");
    printf("Das Programm wird abstürzen wenn der verfügbare Speicher erschöpft ist.\n\n");
    
    allokierungs_zaehler = 0;
    
    /* Endlosschleife die kontinuierlich Speicher allokiert */
    do {
        allokierungs_zaehler++;
        
        /* Versuche 1MB Speicher zu allokieren */
        speicher_zeiger = (char*)malloc(speicher_block_groesse);
        
        /* Prüfe ob die Allokierung erfolgreich war */
        if (speicher_zeiger == NULL) {
            printf("FEHLER: Speicher-Allokierung fehlgeschlagen bei Block %u!\n", 
                   allokierungs_zaehler);
            printf("Verfügbarer Speicher ist erschöpft.\n");
            return 1;
        }
        
        /* Schreibe in den allozierten Speicher um sicherzustellen dass er wirklich verwendet wird */
        memset(speicher_zeiger, 0xAA, speicher_block_groesse);
        
        /* Gebe alle 100 Allokierungen eine Statusmeldung aus */
        if (allokierungs_zaehler % 100 == 0) {
            printf("Block %u erfolgreich allokiert (Gesamtverbrauch: ~%u MB)\n", 
                   allokierungs_zaehler, allokierungs_zaehler);
        }
        
        /* HIER IST DER FEHLER: Wir rufen free() nicht auf! */
        /* free(speicher_zeiger); <-- Diese Zeile würde das Memory Leak beheben */
        
    } while (speicher_zeiger != NULL);
    
    /* Dieser Code wird normalerweise nie erreicht */
    printf("Programm beendet.\n");
    return 0;
}