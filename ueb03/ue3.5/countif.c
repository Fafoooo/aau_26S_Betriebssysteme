// Ü 3.5 - Function Pointer
//
// count_if() wendet einen Function Pointer auf jedes Array-Element an
// und zaehlt wie oft die Funktion true zurueckgibt.

#include <stdio.h>
#include <stdbool.h>

// Function Pointer typedef: Funktion die int nimmt und bool zurueckgibt
typedef bool (*IntFunction)(int);

// Hilfsvariable fuer isGreaterThanPrevious
static int previous = 0;

// 1) isEven: prueft ob Zahl gerade ist
bool isEven(int value)
{
    return value % 2 == 0;
}

// 1) isGreaterThanPrevious: prueft ob Zahl groesser als die vorherige ist
bool isGreaterThanPrevious(int value)
{
    bool result = value > previous;
    previous = value;
    return result;
}

// 2) count_if: wendet func auf jedes Element an, zaehlt true-Ergebnisse
unsigned count_if(int array[], unsigned size, IntFunction func)
{
    unsigned count = 0;
    for (unsigned i = 0; i < size; i++)
    {
        if (func(array[i]))
        {
            count++;
        }
    }
    return count;
}

int main(int argc, char* argv[])
{
    // 3) Test-Array aus der Angabe
    int values[10] = {55, 12, 98, 55, 15, 8, 52, 71, 12, 11};

    // count_if mit isEven aufrufen
    unsigned even_count = count_if(values, 10, isEven);

    // previous zuruecksetzen fuer isGreaterThanPrevious
    previous = 0;
    unsigned greater_count = count_if(values, 10, isGreaterThanPrevious);

    // Erwartetes Ergebnis: isEven: 5   isGreaterThanPrevious: 4
    printf("isEven: %u\t\tisGreaterThanPrevious: %u\n", even_count, greater_count);

    return 0;
}
