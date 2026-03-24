/* getlower.c - ASCII table and lowercase conversion */

#include <stdio.h>
#include <string.h>

void print_ascii_table(void)
{
    printf("%-5s %-5s %s\n", "Dec", "Hex", "Char");
    for (int i = 32; i < 127; i++)
        printf("%-5d 0x%-3X %c\n", i, i, (char)i);
}

char to_lower(char c)
{
    if (c >= 'A' && c <= 'Z')
        return c + 32;
    return c;
}

void convert_to_lower(char *str)
{
    for (int i = 0; str[i]; i++)
        str[i] = to_lower(str[i]);
}

int main(void)
{
    print_ascii_table();

    char buf[256];
    printf("\nEingabe: ");
    if (!fgets(buf, sizeof(buf), stdin))
        return 1;

    char *nl = strchr(buf, '\n');
    if (nl) *nl = '\0';

    convert_to_lower(buf);
    printf("Ausgabe: %s\n", buf);

    return 0;
}
