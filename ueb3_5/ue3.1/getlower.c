// aufgabe 3.1 - ascii tabelle und kleinbuchstaben
#include <stdio.h>
#include <string.h>

int main()
{
  // teil a: ascii tabelle ausgeben
  int i;
  printf("ASCII Tabelle:\n");
  for(i = 32; i <= 126; i++){
      printf("%d = %c\n", i, i);
  }

  // teil b: grossbuchstaben umwandeln
  char text[200];
  printf("\nText eingeben: ");
  fgets(text, 200, stdin);

  // newline weg
  int len = strlen(text);
  if(text[len-1] == '\n')
    text[len-1] = '\0';

  // jedes zeichen pruefen
  for(i = 0; i < strlen(text); i++)
  {
      if(text[i] >= 65 && text[i] <= 90)  // A=65, Z=90
      {
          text[i] = text[i] + 32;  // a=97, differenz ist 32
      }
  }
  printf("Ergebnis: %s\n", text);

  return 0;
}
