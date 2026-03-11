/*
 * Ü 2.2 - Pointer verstehen
 *
 * Originalcode (rekonstruiert aus Angabe) mit Korrekturen und Kommentaren.
 */

#include <stdio.h>
#include <stdlib.h>

int main(void) {

    /* --- Variablen auf dem Stack --- */
    int a = 5;                      // int-Variable a wird mit 5 initialisiert (Stack)
    int b = 10;                     // int-Variable b wird mit 10 initialisiert (Stack)

    /* --- Pointer auf Stack-Variablen --- */
    int *pa = &a;                   // Pointer pa zeigt auf a → *pa == 5
    int *pb = &b;                   // Pointer pb zeigt auf b → *pb == 10

    printf("a = %d\n", a);         // Ausgabe: a = 5
    printf("b = %d\n", b);         // Ausgabe: b = 10
    printf("*pa = %d\n", *pa);     // Dereferenzierung: Ausgabe: *pa = 5
    printf("*pb = %d\n", *pb);     // Dereferenzierung: Ausgabe: *pb = 10

    /* --- Pointer-Arithmetik --- */
    *pa = 20;                       // Wert von a wird über pa auf 20 geändert
    printf("a = %d\n", a);         // Ausgabe: a = 20 (a wurde via Pointer verändert)

    /* --- Speicher auf dem Heap --- */
    /*
     * a) Die Zeile "int *pc = (int *)malloc(sizeof(int));" muss VOR der
     *    ersten Verwendung von pc stehen. Steht sie danach (z.B. nach *pc = 42),
     *    greift man auf nicht allokierten Speicher zu → Absturz (Segfault).
     */
    int *pc = (int *)malloc(sizeof(int));  // Speicher für einen int auf dem Heap reservieren
    *pc = 42;                       // Wert 42 in den Heap-Speicher schreiben
    printf("*pc = %d\n", *pc);     // Ausgabe: *pc = 42

    /* --- sizeof --- */
    /*
     * c) sizeof(int) gibt die Größe eines int-Werts (typisch 4 Bytes).
     *    sizeof(int*) gibt die Größe eines Pointers (typisch 8 Bytes auf 64-Bit).
     *    Auf 32-Bit wäre sizeof(int*) = 4 Bytes.
     *    → Die Werte lassen auf eine 64-Bit-Architektur schließen (x86_64/ARM64).
     */
    printf("sizeof(int) = %zu\n", sizeof(int));    // Ausgabe: 4
    printf("sizeof(int*) = %zu\n", sizeof(int *));  // Ausgabe: 8 (auf 64-Bit)

    /* Heap-Speicher freigeben */
    free(pc);

    /*
     * d) Stack vs. Heap:
     *    - Stack: Automatisch verwalteter Speicher für lokale Variablen.
     *      Wird beim Betreten einer Funktion allokiert und beim Verlassen
     *      automatisch freigegeben. Schnell, aber begrenzt (wenige MB).
     *    - Heap: Dynamisch verwalteter Speicher. Muss explizit mit malloc()
     *      angefordert und mit free() freigegeben werden. Größer, aber langsamer.
     *    - malloc(size_t n) reserviert n Bytes auf dem Heap und gibt einen
     *      void-Pointer auf den Anfang des Blocks zurück. Bei Fehler: NULL.
     *
     * e) Die Ausgaben sind grundsätzlich konsistent für die Stack-Variablen
     *    (a, b, pa, pb), da diese deterministisch initialisiert werden.
     *    Die Adressen selbst (printf("%p", ...)) können sich zwischen Aufrufen
     *    ändern, da moderne Betriebssysteme ASLR (Address Space Layout
     *    Randomization) verwenden - eine Sicherheitsmaßnahme, die die
     *    Speicheradressen bei jedem Programmstart zufällig verschiebt.
     *    Die WERTE bleiben jedoch gleich, da sie explizit gesetzt werden.
     */

    return 0;
}
