/* Generalization of Peterson's algorithm to N threads/processes.
 * Processes are allowed into the section in the order of assigned numbers.
 * In case of identical numbers, the process number decides.
 * Assumption that the computer writes any integer number atomically.
 * */

#include <stdio.h>
#include <stdbool.h>
#include <pthread.h>

#define NUM_THREADS 4

// Indicate whether a thread is entering the critical section
bool entering[NUM_THREADS];
// Store the ticket number assigned to each thread
int num[NUM_THREADS];
int shared_counter = 0;

int max(int arr[], int size) {
  int max = arr[0];
  for (int k = 1; k < size; k++) {
    if (arr[k] > max) {
      max = arr[k];
    }
  }
  return max;
}

void lock(int i) {
  entering[i] = true;
  num[i] = 1 + max(num, N);
  entering[i] = false;

  for (int j = 0; j < NUM_THREADS; j++) {
    while (entering[j]);
    while (num[j] && (num[j] < num[i] || (num[j] == num[i] && j < i)));
  }
}

void unlock(int i) {
  num[i] = 0;
}

void* func(void* arg) {
  int thread_id = *((int*)arg);

  lock(thread_id);

  // Critical Section
  shared_counter++;
  printf("Thread %d incremented counter: %d\n", thread_id, shared_counter);

  unlock(thread_id);

  pthread_exit(NULL);
}

int main() {
  pthread_t threads[NUM_THREADS];
  int thread_ids[NUM_THREADS];

  for (int i = 0; i < NUM_THREADS; i++) {
    thread_ids[i] = i;
    pthread_create(&threads[i], NULL, func, &thread_ids[i]);
  }

  for (int i = 0; i < NUM_THREADS; i++) {
    pthread_join(threads[i], NULL);
  }

  return 0;
}

