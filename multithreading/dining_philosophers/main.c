#include <stdio.h>
#include <stdlib.h>
#include <ncurses.h>
#include <pthread.h>
#include <unistd.h>
#include "utils.h"
#include "text_interface.h"

int NUM_PHILOSOPHERS;
pthread_mutex_t mutex;
pthread_cond_t** conditions;

void* startSim(void* arg);

void grabForks(int id);

void returnForks(int id);

void check(int id);


int main(int argc, char* argv[]) {
  // Initialize NUM_PHILOSOPHERS and validate it
  if (argc != 2) {
    printf("The number of philosophers is missing!");
    return EXIT_FAILURE;
  }

  NUM_PHILOSOPHERS = atoi(argv[1]);

  if (!isNumberInRange(NUM_PHILOSOPHERS)) {
    printf("The number must be in range from 5 to 10!");
    return EXIT_FAILURE;
  }

  // Init Ncurses
  initNcurses();
  WINDOW** windows = drawPhilosophers(NUM_PHILOSOPHERS);

  pthread_mutex_init(&mutex, NULL);

  createPhilosophers(windows, NUM_PHILOSOPHERS);
  conditions = (pthread_cond_t**) calloc(NUM_PHILOSOPHERS,
                                         sizeof(pthread_cond_t*));

  pthread_t threads[NUM_PHILOSOPHERS];

  for (int i = 0; i < NUM_PHILOSOPHERS; i++) {
    conditions[i] = (pthread_cond_t*) malloc(sizeof(pthread_cond_t));
    pthread_cond_init(conditions[i], NULL);
  }

  int ids[NUM_PHILOSOPHERS];

  for (int i = 0; i < NUM_PHILOSOPHERS; i++) {
    ids[i] = i;
    pthread_create(&threads[i], NULL, startSim, (void*) &ids[i]);
    sleep(1);
  }

  for (int i = 0; i < NUM_PHILOSOPHERS; i++) {
    pthread_join(threads[i], NULL);
  }

  /* Cleanup */
  pthread_mutex_destroy(&mutex);
  for (int i = 0; i < NUM_PHILOSOPHERS; i++) {
    pthread_cond_destroy(conditions[i]);
  }

  exitSubWindows(windows, NUM_PHILOSOPHERS);
  exitNcurses();

  return EXIT_SUCCESS;
}

void* startSim(void* arg) {
  while (1) {
    if (getch() != ERR) {
      break;
    }

    int id = *(int*) arg;
    sleep(2);
    grabForks(id);
    returnForks(id);
  }
}

void grabForks(int id) {
  pthread_mutex_lock(&mutex);

  setPhilosopherStatus(id, HUNGRY);
  updateSubWindow(getPhilosopher(id));

  check(id);

  while (getPhilosopherStatus(id) != EATING) {
    pthread_cond_wait(conditions[id], &mutex);
  }

  pthread_mutex_unlock(&mutex);
}

void returnForks(int id) {
  pthread_mutex_lock(&mutex);

  int left = (id + NUM_PHILOSOPHERS - 1) % NUM_PHILOSOPHERS;
  int right = (id + 1) % NUM_PHILOSOPHERS;

  setPhilosopherStatus(id, THINKING);
  updateSubWindow(getPhilosopher(id));
  check(left);
  check(right);

  pthread_mutex_unlock(&mutex);
}

void check(int id) {
  int left = (id + NUM_PHILOSOPHERS - 1) % NUM_PHILOSOPHERS;
  int right = (id + 1) % NUM_PHILOSOPHERS;

  if (getPhilosopherStatus(id) == HUNGRY &&
      getPhilosopherStatus(left) != EATING &&
      getPhilosopherStatus(right) != EATING) {
    setPhilosopherStatus(id, EATING);
    updateSubWindow(getPhilosopher(id));
    sleep(3);
    pthread_cond_signal(conditions[id]);
  }
}