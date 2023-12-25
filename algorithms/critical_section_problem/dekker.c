/* Both of threads incremented the value, but since they did it at the same time, the value was incremented only once.
 * This condition is known as the Race condition in which the resources are being shared concurrently to more than one process.
 * It enables two threads to share a single-use resource without interfering with each other,
 * and it relies solely on shared memory for communication.
 * */

#include <stdbool.h>
#include <stdio.h>
#include <pthread.h>

bool request1 = false;
bool request2 = false;
// Which thread can access the critical section
int granted = 0;
int shared_counter = 0;

void* func1(void* arg) {
  // Private section
  request1 = true;

  // Entry section
  while (request2) {
    if (granted != 1) {
      request1 = false;
      while (granted != 1);
      request1 = true;
    }
  }

  // Critical section
  granted = 2;
  shared_counter++;
  printf("Thread 1 incremented counter: %d\n", shared_counter);

  // Exit section
  request1 = false;

  pthread_exit(NULL);
}

void* func2(void* arg) {
  // Private section
  request2 = true;

  // Entry section
  while (request1) {
    if (granted != 2) {
      request2 = false;
      while (granted != 2);
      request2 = true;
    }
  }

  // Critical section
  granted = 1;
  shared_counter++;
  printf("Thread 2 incremented counter: %d\n", shared_counter);

  // Exit section
  request2 = false;

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