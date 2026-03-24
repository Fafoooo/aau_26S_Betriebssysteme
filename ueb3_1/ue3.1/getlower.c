/*
 * Datei: getlower.c
 * Autor: Student 1
 * Beschreibung: Programm zur Ausgabe der ASCII-Tabelle und Konvertierung 
 *               von Großbuchstaben in Kleinbuchstaben
 */

#include <stdio.h>
#include <string.h>

int main() {
    int ascii_zeichen_nummer;
    char eingabe_string[256];
    int string_index;
    
    /* Teil (a): ASCII-Tabelle ausgeben */
    printf("ASCII-Tabelle (Zeichen 32-126):\n");
    printf("Dec\tHex\tChar\n");
    printf("---\t---\t----\n");
    
    /* Verwende do-while für ASCII-Ausgabe */
    ascii_zeichen_nummer = 32;
    do {
        printf("%d\t%X\t%c\n", ascii_zeichen_nummer, ascii_zeichen_nummer, ascii_zeichen_nummer);
        ascii_zeichen_nummer++;
    } while (ascii_zeichen_nummer <= 126);
    
    printf("\n");
    
    /* Teil (b): String einlesen und Großbuchstaben konvertieren */
    printf("Bitte geben Sie einen String ein: ");
    fgets(eingabe_string, sizeof(eingabe_string), stdin);
    
    /* Entferne das Newline-Zeichen falls vorhanden */
    if (eingabe_string[strlen(eingabe_string) - 1] == '\n') {
        eingabe_string[strlen(eingabe_string) - 1] = '\0';
    }
    
    printf("Originaler String: %s\n", eingabe_string);
    
    /* Konvertiere nur A-Z zu a-z */
    string_index = 0;
    do {
        /* Prüfe ob das aktuelle Zeichen ein Großbuchstabe ist (A-Z) */
        if (eingabe_string[string_index] >= 'A' && eingabe_string[string_index] <= 'Z') {
            /* Konvertiere zu Kleinbuchstabe durch Addition von 32 */
            eingabe_string[string_index] = eingabe_string[string_index] + ('a' - 'A');
        }
        string_index++;
    } while (eingabe_string[string_index] != '\0');
    
    printf("Konvertierter String: %s\n", eingabe_string);
    
    return 0;
}