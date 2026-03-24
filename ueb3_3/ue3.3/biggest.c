/*
 * biggest.c - Maximum Integer Array Size Determination
 * Verwendet binary search mit extensive error checking
 */
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <errno.h>
#include <string.h>

#define MAX_SEARCH_BOUND 2000000000UL  // 2 billion integers max
#define MIN_SEARCH_BOUND 1UL
#define BYTES_PER_MB (1024UL * 1024UL)

// Defensive programming macros
#define SAFE_MALLOC(ptr, count, type) do { \
    (ptr) = (type*)malloc((count) * sizeof(type)); \
    if ((ptr) == NULL && (count) > 0) { \
        fprintf(stderr, "MALLOC FAILED: %zu bytes (errno: %s)\n", \
                (count) * sizeof(type), strerror(errno)); \
    } \
} while(0)

#define SAFE_FREE(ptr) do { \
    if ((ptr) != NULL) { \
        free(ptr); \
        (ptr) = NULL; \
    } \
} while(0)

#define VALIDATE_BOUNDS(low, high) do { \
    if ((low) > (high)) { \
        fprintf(stderr, "ERROR: Invalid bounds - low > high\n"); \
        return EXIT_FAILURE; \
    } \
} while(0)

int main() {
    unsigned long lower_bound = MIN_SEARCH_BOUND;
    unsigned long upper_bound = MAX_SEARCH_BOUND;
    unsigned long middle_value = 0;
    unsigned long max_successful = 0;
    int* test_pointer = NULL;
    unsigned int iteration_count = 0;
    
    printf("=== MAXIMUM INTEGER ARRAY SIZE FINDER ===\n");
    printf("Using binary search algorithm for efficiency\n");
    printf("Search range: %lu to %lu integers\n\n", lower_bound, upper_bound);
    
    // Binary search with bounds validation
    while (lower_bound <= upper_bound) {
        iteration_count++;
        
        // Prevent infinite loops (paranoid check)
        if (iteration_count > 64) {  // log2(2^64) = 64 max iterations
            fprintf(stderr, "ERROR: Too many iterations - aborting\n");
            break;
        }
        
        // Calculate middle point with overflow protection
        middle_value = lower_bound + (upper_bound - lower_bound) / 2;
        
        // Bounds validation
        VALIDATE_BOUNDS(lower_bound, upper_bound);
        
        // Debug output for every test
        printf("Iteration %u: Testing %lu integers (%lu MB)... ", 
               iteration_count, middle_value, 
               (middle_value * sizeof(int)) / BYTES_PER_MB);
        fflush(stdout);
        
        // Attempt allocation with safety checks
        SAFE_MALLOC(test_pointer, middle_value, int);
        
        if (test_pointer != NULL) {
            // Allocation successful
            printf("SUCCESS\n");
            max_successful = middle_value;
            
            // Clean up immediately
            SAFE_FREE(test_pointer);
            
            // Search upper half
            lower_bound = middle_value + 1;
        } else {
            // Allocation failed
            printf("FAILED\n");
            
            // Search lower half
            if (middle_value == 0) {
                break;  // Prevent underflow
            }
            upper_bound = middle_value - 1;
        }
    }
    
    // Results with comprehensive output
    printf("\n=== RESULTS ===\n");
    printf("Maximum allocatable integers: %lu\n", max_successful);
    printf("Memory size: %lu bytes\n", max_successful * sizeof(int));
    printf("Memory size: %.2f MB\n", (double)(max_successful * sizeof(int)) / BYTES_PER_MB);
    printf("Memory size: %.2f GB\n", (double)(max_successful * sizeof(int)) / (BYTES_PER_MB * 1024));
    printf("Iterations required: %u\n", iteration_count);
    
    return EXIT_SUCCESS;
}