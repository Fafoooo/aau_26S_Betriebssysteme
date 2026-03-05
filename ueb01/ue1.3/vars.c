#include <stdio.h>
#include <stdint.h>
#include <string.h>

int main(void) {
    char input[12]; // 10 chars + '\n' + '\0'

    printf("Geben Sie einen String ein (max. 10 Zeichen): ");
    if (fgets(input, sizeof(input), stdin) != NULL) {
        // Remove trailing newline if present
        size_t len = strlen(input);
        if (len > 0 && input[len - 1] == '\n')
            input[len - 1] = '\0';

        // Truncate to 10 characters
        input[10] = '\0';

        printf("Eingabe: %s\n", input);
    }

    printf("\nSpeichergroessen:\n");
    printf("char:           %zu Bytes\n", sizeof(char));
    printf("double:         %zu Bytes\n", sizeof(double));
    printf("unsigned short: %zu Bytes\n", sizeof(unsigned short));
    printf("long:           %zu Bytes\n", sizeof(long));
    printf("uint32_t:       %zu Bytes\n", sizeof(uint32_t));
    printf("uint64_t:       %zu Bytes\n", sizeof(uint64_t));

    return 0;
}
