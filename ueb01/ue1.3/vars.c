#include <stdio.h>
#include <stdint.h>
#include <string.h>

int main(void) {
    char input[12];

    printf("Geben Sie einen String ein (max. 10 Zeichen): ");
    fgets(input, 12, stdin);

    if (input[strlen(input) - 1] == '\n') {
        input[strlen(input) - 1] = '\0';
    }

    input[10] = '\0';

    printf("Eingabe: %s\n", input);

    printf("\nSpeichergroessen:\n");
    printf("char: %zu Bytes\n", sizeof(char));
    printf("double: %zu Bytes\n", sizeof(double));
    printf("unsigned short: %zu Bytes\n", sizeof(unsigned short));
    printf("long: %zu Bytes\n", sizeof(long));
    printf("uint32_t: %zu Bytes\n", sizeof(uint32_t));
    printf("uint64_t: %zu Bytes\n", sizeof(uint64_t));

    return 0;
}