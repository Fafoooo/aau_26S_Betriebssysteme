/*
 * endless.c - Endlosschleife für ulimit demonstration
 * Autor: Student 3
 */
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <signal.h>
#include <unistd.h>

#define PRINT_INTERVAL 25000000ULL
#define INITIAL_MESSAGE_LENGTH 200

// Error handling macro
#define SAFE_PRINTF(...) do { \
    if (printf(__VA_ARGS__) < 0) { \
        fprintf(stderr, "ERROR: printf failed\n"); \
        exit(EXIT_FAILURE); \
    } \
} while(0)

// Signal handler for graceful termination
void signal_handler(int signum) {
    SAFE_PRINTF("\nSignal %d received - terminating gracefully\n", signum);
    exit(signum);
}

int main() {
    unsigned long long counter = 0;
    time_t start_time, current_time;
    
    // Install signal handlers for safety
    if (signal(SIGTERM, signal_handler) == SIG_ERR) {
        fprintf(stderr, "WARNING: Could not install SIGTERM handler\n");
    }
    if (signal(SIGINT, signal_handler) == SIG_ERR) {
        fprintf(stderr, "WARNING: Could not install SIGINT handler\n");
    }
    
    // Get start time - with error checking
    start_time = time(NULL);
    if (start_time == (time_t)-1) {
        fprintf(stderr, "ERROR: time() failed\n");
        return EXIT_FAILURE;
    }
    
    SAFE_PRINTF("Endlosschleife gestartet...\n");
    SAFE_PRINTF("Use: ulimit -t 4 && ./endless\n");
    SAFE_PRINTF("Das Programm wird nach 4 Sekunden durch SIGXCPU beendet.\n\n");
    
    // Infinite loop with defensive programming
    while (1) {
        counter++;
        
        // Overflow check (paranoid but safe)
        if (counter == 0) {
            fprintf(stderr, "WARNING: Counter overflow detected!\n");
            break;
        }
        
        // Periodic status updates
        if (counter % PRINT_INTERVAL == 0) {
            current_time = time(NULL);
            if (current_time != (time_t)-1) {
                SAFE_PRINTF("Iterationen: %llu, Zeit: %ld Sekunden\n", 
                           counter, current_time - start_time);
            }
        }
    }
    
    // This should never be reached unless overflow
    SAFE_PRINTF("Unerwartetes Ende der Schleife!\n");
    return EXIT_SUCCESS;
}