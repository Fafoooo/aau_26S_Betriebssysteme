// Ü 2.2 - Pointer verstehen

#include <stdio.h>
#include <stdlib.h>

int main(int argc, char* argv[])
{
    int  a = 4;             // Variable a mit Wert 4
    int* b = NULL;          // Pointer b, zeigt noch nirgends hin

    // Gibt a=4, Adresse von a, und nochmal 4 aus (*(&a) = Wert an Adresse von a)
    printf("\n0: a=%d &a=%p *(&a)=%d", a, &a, *(&a));
    // Gibt b=0x0 (NULL) und die Adresse von b selbst aus
    printf("\n0: b=%p &b=%p\n", b, &b);

    // Reserviert Platz fuer 4 ints auf dem Heap. b und c zeigen beide drauf.
    int* c = b = (int*) malloc(sizeof(int) * 4);
    *b         = 10;        // Schreibt 10 ins erste Element -> [10, ?, ?, ?]

    // b und c haben dieselbe Adresse, Wert ist 10
    printf("\n1: b=%p *b=%d &b=%p", b, *b, &b);
    printf("\n1: c=%p *c=%d &c=%p\n", c, *c, &c);

    b[2] = 20;              // Drittes Element = 20 -> [10, ?, 20, ?]
    b++;                     // b rueckt eins weiter, zeigt jetzt auf zweites Element
    *(b + 2) = 30;          // b+2 = viertes Element = 30 -> [10, ?, 20, 30]
    *(b--)   = 40;          // Schreibt 40 ins zweite Element, dann b zurueck -> [10, 40, 20, 30]

    // Geht alle 4 Elemente durch und gibt sie aus: 10, 40, 20, 30
    for (int i = 0, *pi = c; i < 4; ++i, ++pi)
    {
        printf("\n2: b[%ld]=%d (addr=%p)", pi - c, *pi, pi);
    }
    printf("\n");

    // a) FIX: Im Original stand b = &a VOR free(b), was einen Absturz verursacht,
    //    weil free() nur Heap-Speicher freigeben darf, nicht Stack. Zeilen getauscht:
    free(b);                 // Heap-Speicher freigeben (b zeigt noch auf Heap)
    b = &a;                  // b zeigt jetzt auf a (Stack)

    // b zeigt auf a, also *b = 4
    printf("\n3: a=%d &a=%p *(&a)=%d", a, &a, *(&a));
    printf("\n3: b=%p *b=%d &b=%p\n", b, *b, &b);

    // Gibt Groesse von int (4 Bytes) und Groesse von Pointer (8 Bytes) aus
    printf("\n4: size(int)=%zu, size(int*)=%zu\n", sizeof(*b), sizeof(b));


    // c) sizeof(int) gibt 4 aus - ein int braucht 4 Bytes (32 Bit).
    //    sizeof(int*) gibt 8 aus - ein Pointer braucht 8 Bytes (64 Bit).
    //    Ein Pointer speichert eine Speicheradresse. Auf einem 64-Bit-System
    //    sind Adressen 64 Bit lang, daher 8 Bytes pro Pointer.
    //    Auf einem 32-Bit-System waeren es nur 4 Bytes.
    //    -> sizeof(int*) = 8 bedeutet: 64-Bit-Architektur.

    // d) Stack: Hier landen lokale Variablen (z.B. int a = 4). Der Speicher wird
    //    automatisch freigegeben wenn die Funktion endet. Schnell, aber begrenzt.
    //    Heap: Hier holt man sich Speicher selbst mit malloc(). Man muss ihn auch
    //    selbst wieder freigeben mit free(). Grösser als der Stack, aber langsamer.
    //    malloc(n) reserviert n Bytes auf dem Heap und gibt einen Pointer drauf zurück.
    //    Wenn kein Platz mehr da ist, gibt malloc NULL zurück.

    // e) Werte (4, 10, 40, 20, 30) sind jedes Mal gleich, weil wir sie selbst setzen.
    //    Die Adressen ändern sich aber bei jedem Aufruf.

    return EXIT_SUCCESS;
}
