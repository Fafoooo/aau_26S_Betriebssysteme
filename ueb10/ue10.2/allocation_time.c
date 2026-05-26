// Ü 10.2 - Stack vs. Heap Allocation Performance
//
//
// === Hintergrund: wann Stack, wann Heap? ===
//
// Stack: Lokale Variablen und Funktionsparameter liegen am Stack. Eine
//        "Allokation" ist nur das Verschieben des Stack-Pointers,
//        praktisch eine einzige CPU-Instruktion beim Betreten einer
//        Funktion. Sehr schnell, aber die Größe muss meist zur
//        Compile-Zeit bekannt sein, und der Speicher lebt nur bis zum
//        Ende des Blocks/der Funktion.
//
// Heap: malloc/free fordern Speicher zur Laufzeit an. Der Allocator muss
//       einen passenden freien Block suchen, Buchhaltung führen und sich
//       ggf. beim OS neuen Speicher holen (sbrk/mmap). Deutlich teurer,
//       dafür frei wählbar in Größe und Lebensdauer.
//
// Erwartung: Stack-Allokationen sind deutlich schneller als Heap-
// Allokationen.
//
//
// === Kompilieren ===
//
//   gcc -O0 allocation_time.c -o allocation_time
//   ./allocation_time
//
// Wichtig: mit -O0 kompilieren, damit der Compiler die Schleifen und
// die (eigentlich folgenlosen) Stack-Puffer nicht wegoptimiert.

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define ITERATIONS 1000000
#define BLOCK_SIZE 1024

// Bei jedem Aufruf wird ein Stack-Frame mit einem Puffer angelegt und
// beim Return wieder freigegeben. volatile + Schreibzugriff verhindern,
// dass der Compiler den Puffer komplett wegoptimiert.
static void stack_block(int i)
{
    volatile char buffer[BLOCK_SIZE];
    buffer[0] = (char) i;
}

void StackAllocationTime(void)
{
    clock_t start = clock();

    for (int i = 0; i < ITERATIONS; i++)
    {
        stack_block(i);
    }

    clock_t end = clock();
    double seconds = (double)(end - start) / CLOCKS_PER_SEC;
    printf("Time of %d stack allocations: %f seconds\n", ITERATIONS, seconds);
}

void HeapAllocationTime(void)
{
    clock_t start = clock();

    for (int i = 0; i < ITERATIONS; i++)
    {
        char *buffer = malloc(BLOCK_SIZE);
        if (buffer == NULL)
        {
            perror("malloc");
            exit(EXIT_FAILURE);
        }
        buffer[0] = (char) i;
        free(buffer);
    }

    clock_t end = clock();
    double seconds = (double)(end - start) / CLOCKS_PER_SEC;
    printf("Time of %d heap allocations: %f seconds\n", ITERATIONS, seconds);
}

int main(void)
{
    StackAllocationTime();
    HeapAllocationTime();
    return 0;
}
