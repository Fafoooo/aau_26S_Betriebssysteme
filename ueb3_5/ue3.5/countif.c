// function pointer aufgabe
#include <stdio.h>
#include <stdbool.h>

typedef bool (*IntFunction)(int);

int letzteZahl = 0;  // fuer isGreaterThanPrevious

// prueft ob zahl gerade ist
bool isEven(int zahl)
{
    if(zahl % 2 == 0)
        return true;
    else
        return false;
}

// prueft ob zahl groesser als letzte ist
bool isGreaterThanPrevious(int zahl)
{
    bool erg;
    if(zahl > letzteZahl)
        erg = true;
    else
        erg = false;
    letzteZahl = zahl;
    return erg;
}

// zaehlt wie oft die funktion true zurueckgibt
unsigned count_if(int arr[], unsigned size, IntFunction func)
{
    unsigned count = 0;
    int i;
    for(i = 0; i < size; i++){
        if(func(arr[i]) == true){
            count++;
        }
    }
    return count;
}

int main()
{
    int werte[10] = {55, 12, 98, 55, 15, 8, 52, 71, 12, 11};

    unsigned gerade = count_if(werte, 10, isEven);

    letzteZahl = 0;  // zuruecksetzen
    unsigned groesser = count_if(werte, 10, isGreaterThanPrevious);

    printf("isEven: %u\t\tisGreaterThanPrevious: %u\n", gerade, groesser);

    return 0;
}
