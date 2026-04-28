// Ü 7.4 - Spaltensummen einer NxN-Matrix mit fork()
//
// Eine 100x100-Matrix mit Zufallswerten wird angelegt. 100 Kindprozesse
// berechnen jeweils die Summe einer Spalte (jedes Kind eine andere)
// und geben sie aus.
//
// Vergleichs-Pendant: threadmatrix.c
// Laufzeit messen mit:  time ./forkmatrix
//
// Kompilieren:  gcc -O2 -o forkmatrix forkmatrix.c

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <time.h>

#define N 100

// static = liegt im BSS, nicht auf dem Stack (sonst Overflow bei großem N)
static int matrix[N][N];

int main(void)
{
    srand((unsigned)time(NULL));
    for (int i = 0; i < N; i++)
        for (int j = 0; j < N; j++)
            matrix[i][j] = rand() % 100;

    for (int col = 0; col < N; col++) {
        if (fork() == 0) {
            // Kind: nur diese Spalte aufsummieren, dann beenden
            long sum = 0;
            for (int row = 0; row < N; row++)
                sum += matrix[row][col];
            printf("col %3d sum = %ld\n", col, sum);
            exit(EXIT_SUCCESS);
        }
        // Eltern macht in der Schleife mit dem nächsten col weiter
    }

    while (wait(NULL) > 0);  // alle Kinder einsammeln, keine Zombies
    return EXIT_SUCCESS;
}
