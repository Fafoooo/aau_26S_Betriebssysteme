// Ü 3.2a - Resource Limits: Endlosschleife
//
// Kompilieren: gcc -o endless endless.c
// Ausfuehren mit Zeitlimit:
//   ulimit -t 4       (CPU-Zeit auf 4 Sekunden beschraenken)
//   ./endless          (wird nach 4s vom OS beendet mit SIGKILL/SIGXCPU)
//
// ulimit -t setzt das CPU-Zeitlimit (in Sekunden) fuer die aktuelle Shell.
// Nach Ablauf sendet das OS SIGXCPU und dann SIGKILL an den Prozess.

#include <stdio.h>
#include <stdlib.h>

int main(int argc, char* argv[])
{
    printf("Endlosschleife gestartet...\n");
    printf("Beenden mit: ulimit -t 4 (vorher setzen) oder Ctrl+C\n");

    unsigned long long counter = 0;
    while (1)
    {
        counter++;
        // Alle 100 Millionen Iterationen Lebenszeichen
        if (counter % 100000000 == 0)
        {
            printf("Noch am Leben... (%llu Iterationen)\n", counter);
        }
    }

    return EXIT_SUCCESS;
}
