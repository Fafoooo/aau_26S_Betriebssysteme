// Implementierung der in add.h deklarierten add-Funktion.
//
// Diese Datei FEHLTE im ursprünglichen Angabe-Paket. Genau deshalb
// schlug "make" im Originalzustand fehl: das Makefile verlangt add.o,
// das aus add.c gebaut wird, aber add.c war nicht vorhanden.

#include "add.h"

int add(int a, int b)
{
    return a + b;
}
