// Ü 3.1 - Kleinbuchstaben für char[]
//
// a) ASCII-Tabelle ausgeben (char + int-Wert)
// b) Beliebige Zeichenkette einlesen, Grossbuchstaben in Kleinbuchstaben umwandeln

#include <stdio.h>
#include <string.h>

#define MAX_INPUT 256

int main(int argc, char* argv[])
{
    // --- a) ASCII-Tabelle ---
    printf("=== ASCII-Tabelle ===\n");
    printf("%-6s %-6s %s\n", "Dez", "Hex", "Char");
    printf("--------------------\n");

    for (int i = 32; i < 127; i++)  // 32 (Space) bis 126 (~), druckbare Zeichen
    {
        printf("%-6d 0x%-4X %c\n", i, i, (char)i);
    }

    // --- b) Eingabe -> Kleinbuchstaben ---
    printf("\n=== Eingabe -> Kleinbuchstaben ===\n");
    printf("Eingabe: ");

    char input[MAX_INPUT];
    if (fgets(input, MAX_INPUT, stdin) == NULL)
        return 1;

    // Newline entfernen
    char *nl = strchr(input, '\n');
    if (nl) *nl = '\0';

    // Nur Grossbuchstaben (A-Z) umwandeln, alles andere bleibt gleich.
    // 'A' = 65, 'Z' = 90, 'a' = 97 -> Differenz ist 32
    for (int i = 0; input[i] != '\0'; i++)
    {
        if (input[i] >= 'A' && input[i] <= 'Z')
        {
            input[i] = input[i] + ('a' - 'A');  // +32
        }
    }

    printf("Ausgabe: %s\n", input);

    return 0;
}
