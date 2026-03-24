// groesstes moeglihces int array finden
#include <stdio.h>
#include <stdlib.h>

int main()
{
    // erstmal rausfinden wo ungefaehr die grenze ist
    size_t klein = 1;
    size_t gross = 1;

    while(1){
        int *test = malloc(sizeof(int) * gross);
        if(test == NULL)
            break;
        free(test);
        klein = gross;
        gross = gross * 2;
    }

    // jetzt binaere suche zwischen klein und gross
    while(klein + 1 < gross)
    {
        size_t mitte = klein + (gross - klein) / 2;
        int *test = malloc(sizeof(int) * mitte);
        if(test != NULL){
            free(test);
            klein = mitte;
        } else {
            gross = mitte;
        }
    }

    printf("maximale groesse: %zu elemente\n", klein);
    printf("das sind ungefaehr %.2f GB\n",
        (double)(klein * sizeof(int)) / (1024.0*1024.0*1024.0));

    return 0;
}
