/*
 * Datei: countif.c
 * Beschreibung: Implementierung einer count_if Funktion mit Function Pointern
 *               Testet verschiedene Praedikat-Funktionen auf einem Integer-Array
 */

#include <stdio.h>
#include <stdbool.h>

/* Typdefinition fuer Funktionszeiger */
typedef bool (*IntFunction)(int);

/* Globale Variable fuer vorheriges Element */
static int vorheriges_element = 0;

/* Funktion zur Zaehlung von Array-Elementen die ein Praedikat erfuellen */
int count_if(int* array_zeiger, int array_groesse, IntFunction praedikat_funktion) {
    int anzahl_treffer = 0;
    int index = 0;

    /* Durchlaufe alle Elemente mit do-while */
    do {
        if (praedikat_funktion(array_zeiger[index])) {
            anzahl_treffer++;
        }
        index++;
    } while (index < array_groesse);

    return anzahl_treffer;
}

/* Prueft ob eine Zahl gerade ist */
bool isEven(int zahl) {
    return (zahl % 2) == 0;
}

/* Prueft ob aktuelle Zahl groesser als die vorherige ist */
bool isGreaterThanPrevious(int aktueller_wert) {
    bool ergebnis = aktueller_wert > vorheriges_element;
    vorheriges_element = aktueller_wert;
    return ergebnis;
}

int main() {
    int test_array[] = {55, 12, 98, 55, 15, 8, 52, 71, 12, 11};
    int laenge = sizeof(test_array) / sizeof(test_array[0]);

    int gerade_anzahl = count_if(test_array, laenge, isEven);

    /* Vorheriges Element zuruecksetzen vor zweitem Aufruf */
    vorheriges_element = 0;
    int groessere_anzahl = count_if(test_array, laenge, isGreaterThanPrevious);

    printf("isEven: %d\t\tisGreaterThanPrevious: %d\n", gerade_anzahl, groessere_anzahl);

    return 0;
}
