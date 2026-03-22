// Ü 3.1 - ASCII-Tabelle und Kleinbuchstaben

#include <stdio.h>
#include <string.h>

int main(void)
{
    // a) ASCII-Tabelle
    printf("Dez\tChar\n");
    printf("----------\n");
    for (int i = 32; i < 127; i++)
        printf("%d\t%c\n", i, i);

    // b) Eingabe in Kleinbuchstaben umwandeln
    printf("\nEingabe: ");
    char input[256];
    if (fgets(input, sizeof(input), stdin) == NULL)
        return 1;

    size_t len = strlen(input);
    if (len > 0 && input[len - 1] == '\n')
        input[len - 1] = '\0';

    for (int i = 0; input[i] != '\0'; i++)
    {
        if (input[i] >= 'A' && input[i] <= 'Z')
            input[i] += 'a' - 'A';
    }

    printf("Ausgabe: %s\n", input);
    return 0;
}
