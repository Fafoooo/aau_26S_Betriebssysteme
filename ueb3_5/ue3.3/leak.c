// speicherleck programm
// malloc ohne free -> irgendwann kein speicher mehr
#include <stdio.h>
#include <stdlib.h>

int main()
{
    int zaehler = 0;

    while(1)
    {
        int *ptr = malloc(sizeof(int) * 1000);

        // pruefen ob malloc geklappt hat
        if(ptr == NULL){
            printf("kein speicher mehr nach %d malloc aufrufen\n", zaehler);
            break;
        }

        // eigentlich muesste man hier free(ptr) machen
        // sonst ist der speicher weg (memory leak)
        free(ptr);  // loesung: speicher wieder freigeben

        zaehler++;
    }
    return 0;
}
