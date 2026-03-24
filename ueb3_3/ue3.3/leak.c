/*
 * leak.c - Memory Leak Demonstration with defensive programming
 * Author: Student 3
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#define BLOCK_SIZE (1024 * 1024)  // 1 MB per allocation
#define STATUS_INTERVAL 50
#define MAX_ALLOCATIONS 10000  // Safety limit

// Defensive error checking macro
#define CHECK_ALLOC(ptr, size) do { \
    if ((ptr) == NULL) { \
        fprintf(stderr, "ALLOCATION FAILED: Could not allocate %zu bytes\n", (size_t)(size)); \
        fprintf(stderr, "Error: %s\n", strerror(errno)); \
        return EXIT_FAILURE; \
    } \
} while(0)

// Status reporting macro
#define REPORT_STATUS(count, total_mb) do { \
    if ((count) % STATUS_INTERVAL == 0) { \
        printf("Successfully allocated: %u blocks (%u MB total)\n", (count), (total_mb)); \
        fflush(stdout); \
    } \
} while(0)

int main() {
    unsigned int allocation_count = 0;
    char* memory_pointer = NULL;
    unsigned int total_megabytes = 0;
    
    printf("=== MEMORY LEAK DEMONSTRATION ===\n");
    printf("Allocating %d KB blocks without calling free()\n", BLOCK_SIZE / 1024);
    printf("Program will crash when system memory is exhausted.\n");
    printf("This demonstrates the importance of proper memory management.\n\n");
    
    // Main allocation loop - defensive approach
    while (allocation_count < MAX_ALLOCATIONS) {
        allocation_count++;
        
        // Attempt memory allocation
        memory_pointer = (char*)malloc(BLOCK_SIZE);
        
        // Critical: Check if allocation succeeded
        CHECK_ALLOC(memory_pointer, BLOCK_SIZE);
        
        // Initialize memory to ensure it's actually committed
        // This prevents lazy allocation optimizations
        memset(memory_pointer, 0xDE, BLOCK_SIZE);  // 0xDE = "Dead" memory pattern
        
        total_megabytes = allocation_count;
        
        // Periodic status reporting
        REPORT_STATUS(allocation_count, total_megabytes);
        
        // Safety check for runaway allocations
        if (allocation_count >= MAX_ALLOCATIONS) {
            printf("WARNING: Reached maximum allocation limit (%d)\n", MAX_ALLOCATIONS);
            break;
        }
        
        /*
         * CRITICAL BUG - INTENTIONAL MEMORY LEAK:
         * We do NOT call free(memory_pointer) here!
         * This causes all allocated memory to remain allocated,
         * leading to memory exhaustion.
         * 
         * SOLUTION would be:
         * free(memory_pointer);
         * memory_pointer = NULL;  // Good practice
         */
    }
    
    printf("\nProgram completed. Total allocated: %u MB\n", total_megabytes);
    printf("Note: All memory remains allocated (leaked) until process termination.\n");
    
    return EXIT_SUCCESS;
}