#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <time.h>

// One mutex per resource R1-R4
pthread_mutex_t res_mutex[4];

typedef struct {
	int thread_id;
	int num_needs;
	int needs[3];  // max 3 resources needed per thread
} thread_arg_t;

// Shuffle an array of ints in place
void shuffle(int *arr, int n) {
	for (int i = n-1; i > 0; --i) {
		int j = rand() % (i+1);
		int tmp = arr[i];
		arr[i] = arr[j];
		arr[j] = tmp;
	}
}

void *thread_func(void *arg) {
	thread_arg_t *t = (thread_arg_t *)arg;
	int order[3];
	
	for (int i = 0; i < t->num_needs; ++i) {
		order[i] = t->needs[i];
	}
	
	shuffle(order, t->num_needs);

	printf("T%d attempting locks in order:", t->thread_id);
	for (int i = 0; i < t->num_needs; ++i)
		printf(" R%d", order[i]+1);
	printf("\n");

	for (int i = 0; i < t->num_needs; ++i) {
		int r = order[i];
		pthread_mutex_lock(&res_mutex[r]);
		printf("T%d locked R%d\n", t->thread_id, r+1);
		usleep(100000);  // simulate 100 ms access
	}

	printf("T%d has all resources, doing work...\n", t->thread_id);
	usleep(100000);

	for (int i = 0; i < t->num_needs; ++i) {
		int r = order[i];
		pthread_mutex_unlock(&res_mutex[r]);
		printf("T%d unlocked R%d\n", t->thread_id, r+1);
	}

	return NULL;
}

int main(void) 
{
	srand(time(NULL));

	for (int i = 0; i < 4; ++i)
		pthread_mutex_init(&res_mutex[i], NULL);

	// Define each thread's resource needs (id, number, resources)
	thread_arg_t args[4] = {
		{1, 3, {0,1,2}},  // T1 needs R1,R2,R3
		{2, 2, {2,3}},    // T2 needs R3,R4
		{3, 2, {0,3}},    // T3 needs R1,R4
		{4, 2, {0,3}}     // T4 needs R1,R4
	};

	pthread_t th[4];
	for (int i = 0; i < 4; ++i) {
		if (pthread_create(&th[i], NULL, thread_func, &args[i]) != 0) {
			perror("pthread_create");
			exit(1);
		}
	}

	for (int i = 0; i < 4; ++i) 
	{
		pthread_join(th[i], NULL);
	}

	for (int i = 0; i < 4; ++i)
	{
		pthread_mutex_destroy(&res_mutex[i]);
	}

	return 0;
}
