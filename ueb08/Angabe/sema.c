#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

#define NUM_THREADS 1000

// Global shared variables
int shared_resource_1 = 0;
int shared_resource_2 = 0;

// Semaphores
sem_t semaphore_1;
sem_t semaphore_2;

// Function executed by threads
void *thread_function(void *arg) {
    int thread_id = *((int *)arg);
	// Entering critical section for shared_resource_1
	sem_wait(&semaphore_1);    
	printf("Thread %d entered critical section for shared_resource_1\n", thread_id);
    while(1){
        // Modify shared_resource_1
        shared_resource_1 += thread_id;
        printf("Thread %d modified shared_resource_1. New value: %d\n", thread_id, shared_resource_1);
		sem_wait(&semaphore_2);
		printf("Thread %d entered critical section for shared_resource_2\n", thread_id);
        printf("Thread %d will leave critical section for shared_resource_1\n", thread_id);
        sem_post(&semaphore_1);
        // Modify shared_resource_2
        shared_resource_2 += thread_id * 2;
        printf("Thread %d modified shared_resource_2. New value: %d\n", thread_id, shared_resource_2);
        sem_wait(&semaphore_1);
        printf("Thread %d entered critical section for shared_resource_1\n", thread_id);
        printf("Thread %d will leave critical section for shared_resource_2\n", thread_id);
		sem_post(&semaphore_2);
    }
    pthread_exit(NULL);
}

int main() {
    // Initialize semaphores
    sem_init(&semaphore_1, 0, 1);
    sem_init(&semaphore_2, 0, 1);

    // Create threads
    pthread_t threads[NUM_THREADS];
    int thread_ids[NUM_THREADS];
    for (int i = 0; i < NUM_THREADS; i++) {
        thread_ids[i] = i + 1;
        pthread_create(&threads[i], NULL, thread_function, &thread_ids[i]);
    }

    // Join threads
    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_join(threads[i], NULL);
    }

    // Destroy semaphores
    sem_destroy(&semaphore_1);
    sem_destroy(&semaphore_2);

    return 0;
}


