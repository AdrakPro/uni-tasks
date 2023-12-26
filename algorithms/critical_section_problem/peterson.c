// MutEx algorithm for two threads, "ensures" no starvation and jamming

#include <stdio.h>
#include <stdbool.h>
#include <pthread.h>

bool request1 = false;
bool request2 = false;
int granted = 0;
int shared_counter = 0;

void* func1(void* arg) {
  request1 = true;
  granted = 1;

  while (request2 && granted == 1);

  // Critical Section
  shared_counter++;
  printf("Thread 1 incremented counter: %d\n", shared_counter);

  request1 = false;

  // Exit section
  pthread_exit(NULL);
}

void* func2(void* arg) {
  request2 = true;
  granted = 2;

  while (request1 && granted == 2);

  // Critical Section
  shared_counter++;
  printf("Thread 2 incremented counter: %d\n", shared_counter);

  request2 = false;

  // Exit section
  pthread_exit(NULL);
}

int main() {
  pthread_t thread1, thread2;

  pthread_create(&thread1, NULL, func1, NULL);
  pthread_create(&thread2, NULL, func2, NULL);

  pthread_join(thread1, NULL);
  pthread_join(thread2, NULL);

  return 0;
}
