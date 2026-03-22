// Ü 3.2a - Endlosschleife
// Ausfuehren: ulimit -t 4 && ./endless

#include <stdio.h>

int main(void)
{
    printf("Endlosschleife gestartet...\n");
    while (1)
        ;
    return 0;
}
