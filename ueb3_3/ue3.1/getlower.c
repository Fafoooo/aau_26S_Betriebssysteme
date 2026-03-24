/*
 * getlower.c - ASCII Tabelle und Case Conversion
 * Author: Student 3
 */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define ASCII_START 32
#define ASCII_END 126
#define MAX_INPUT_LENGTH 256
#define UPPERCASE_START 'A'
#define UPPERCASE_END 'Z'
#define CASE_OFFSET ('a' - 'A')

// Safety check macro
#define CHECK_NULL(ptr) do { \
    if ((ptr) == NULL) { \
        fprintf(stderr, "ERROR: Null pointer detected!\n"); \
        exit(EXIT_FAILURE); \
    } \
} while(0)

int main() {
    char input_buffer[MAX_INPUT_LENGTH];
    int ascii_value;
    size_t string_length;
    
    // ASCII table output - defensiv programmiert
    printf("ASCII-Tabelle von %d bis %d:\n", ASCII_START, ASCII_END);
    printf("Dec\tHex\tChar\n");
    printf("---\t---\t----\n");
    
    for (ascii_value = ASCII_START; ascii_value <= ASCII_END; ascii_value++) {
        // Bounds check (paranoid but safe)
        if (ascii_value < 0 || ascii_value > 127) {
            fprintf(stderr, "WARNING: ASCII value out of range: %d\n", ascii_value);
            continue;
        }
        printf("%d\t%X\t%c\n", ascii_value, ascii_value, ascii_value);
    }
    
    printf("\n");
    
    // String input with extensive error checking
    printf("Bitte String eingeben (max %d Zeichen): ", MAX_INPUT_LENGTH - 1);
    
    if (fgets(input_buffer, sizeof(input_buffer), stdin) == NULL) {
        fprintf(stderr, "ERROR: Failed to read input!\n");
        return EXIT_FAILURE;
    }
    
    // Remove newline - defensive approach
    string_length = strlen(input_buffer);
    if (string_length > 0 && input_buffer[string_length - 1] == '\n') {
        input_buffer[string_length - 1] = '\0';
        string_length--;
    }
    
    // Check for empty input
    if (string_length == 0) {
        fprintf(stderr, "WARNING: Empty input detected!\n");
        return EXIT_SUCCESS;
    }
    
    printf("Original: %s\n", input_buffer);
    
    // Convert uppercase to lowercase with bounds checking
    for (size_t i = 0; i < string_length; i++) {
        // Multiple safety checks
        if (i >= MAX_INPUT_LENGTH) {
            fprintf(stderr, "ERROR: Array bounds exceeded!\n");
            break;
        }
        
        // Only convert A-Z characters
        if (input_buffer[i] >= UPPERCASE_START && input_buffer[i] <= UPPERCASE_END) {
            input_buffer[i] = input_buffer[i] + CASE_OFFSET;
        }
    }
    
    printf("Konvertiert: %s\n", input_buffer);
    
    return EXIT_SUCCESS;
}