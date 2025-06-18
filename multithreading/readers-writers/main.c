#include <pthread.h>
#include <stdio.h>
#include <semaphore.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>

#define NUM_READERS 5
#define NUM_WRITERS 3

sem_t rw_mutex;  // Controls access to shared resource
sem_t mutex;     // Controls access to read count
int read_count = 0;  // Number of active readers
int data = 0;  // Shared resource

void* reader(void* arg) {
    int id = *(int*)arg;
    while (1) {
        sem_wait(&mutex);
        read_count++;
        if (read_count == 1)
            sem_wait(&rw_mutex);
        sem_post(&mutex);

        // Critical section
        sleep(0.5);
        printf("Reader %d: Read data = %d\n", id, data);
        sleep(1);

        sem_wait(&mutex);
        read_count--;
        if (read_count == 0)
            sem_post(&rw_mutex);
        sem_post(&mutex);

        sleep(2);
    }
}

void* writer(void* arg) {
    int id = *(int*)arg;
    while (1) {
        sem_wait(&rw_mutex);

        // Critical section
        data++;
        printf("Writer %d: Wrote data = %d\n", id, data);
        sleep(1);

        sem_post(&rw_mutex);
        sleep(2);  // Simulate time before writing again
    }
}

int main() {
	srand(time(NULL));

	pthread_t readers[NUM_READERS], writers[NUM_WRITERS];
	int r_ids[NUM_READERS], w_ids[NUM_WRITERS];

    sem_init(&rw_mutex, 0, 1);
    sem_init(&mutex, 0, 1);

	for (int i = 0; i < NUM_READERS; i++) {
		r_ids[i] = i + 1;
		if (pthread_create(&readers[i], NULL, reader, &r_ids[i]) != 0) {
			perror("Failed to create reader thread");
			exit(EXIT_FAILURE);
		}
	}

	for (int i = 0; i < NUM_WRITERS; i++) {
		w_ids[i] = i + 1;
		if (pthread_create(&writers[i], NULL, writer, &w_ids[i]) != 0) {
			perror("Failed to create writer thread");
			exit(EXIT_FAILURE);
		}
	}

	for (int i = 0; i < NUM_READERS; i++) {
		pthread_join(readers[i], NULL);
	}


	for (int i = 0; i < NUM_WRITERS; i++) {
		pthread_join(writers[i], NULL);
	}

    sem_destroy(&rw_mutex);
    sem_destroy(&mutex);

	return EXIT_SUCCESS;
}
