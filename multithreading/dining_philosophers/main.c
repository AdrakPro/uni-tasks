#include <stdio.h>
#include <stdlib.h>
#include <ncurses.h>
#include <pthread.h>
#include <unistd.h>
#include "utils.h"
#include "text_interface.h"

int num_philosophers;
pthread_mutex_t mutex;
pthread_cond_t** conditions;

void* start_sim(void* arg);

void grab_forks(int tid);

void return_forks(int tid);

void check(int tid);

int main(int argc, char* argv[]) {
  // Initialize num_philosophers and validate it
  if (argc != 2) {
    printf("The number of philosophers is missing!\n");
    return EXIT_FAILURE;
  }

  num_philosophers = atoi(argv[1]);

  if (!is_number_in_range(num_philosophers)) {
    printf("The number must be in range from 5 to 10!\n");
    return EXIT_FAILURE;
  }

  // Init Ncurses
  init_ncurses();
  WINDOW** windows = draw_philosophers(num_philosophers);

  // Init mutex
  pthread_mutex_init(&mutex, NULL);

  // Fill the simple database with philosophers
  create_philosophers(windows, num_philosophers);

  // Dynamic allocation, because we are waiting for num_philosophers
  conditions = (pthread_cond_t**) calloc(num_philosophers,
                                         sizeof(pthread_cond_t*));

  for (int i = 0; i < num_philosophers; i++) {
    conditions[i] = (pthread_cond_t*) malloc(sizeof(pthread_cond_t));
    pthread_cond_init(conditions[i], NULL);
  }

  pthread_t threads[num_philosophers];

  int tids[num_philosophers];

  for (int i = 0; i < num_philosophers; i++) {
    tids[i] = i;
    pthread_create(&threads[i], NULL, start_sim, (void*) &tids[i]);
  }

  for (int i = 0; i < num_philosophers; i++) {
    pthread_join(threads[i], NULL);
  }

  // Cleanup
  pthread_mutex_destroy(&mutex);
  for (int i = 0; i < num_philosophers; i++) {
    pthread_cond_destroy(conditions[i]);
  }
  free(conditions);

  exit_sub_windows(windows, num_philosophers);
  exit_ncurses();

  return EXIT_SUCCESS;
}

void* start_sim(void* arg) {
  while (1) {
    if (getch() != ERR) {
      break;
    }

    int id = *(int*) arg;
    sleep(1);
    grab_forks(id);
    return_forks(id);
  }

  return NULL;
}

void grab_forks(int tid) {
  pthread_mutex_lock(&mutex);

  set_philosopher_status(tid, HUNGRY);
  update_sub_window(tid);

  check(tid);

  while (get_philosopher_status(tid) != EATING) {
    pthread_cond_wait(conditions[tid], &mutex);
  }

  pthread_mutex_unlock(&mutex);
  sleep(1);
}

void return_forks(int tid) {
  int left = (tid + num_philosophers - 1) % num_philosophers;
  int right = (tid + 1) % num_philosophers;

  pthread_mutex_lock(&mutex);

  set_philosopher_status(tid, THINKING);
  update_sub_window(tid);

  // Signal to neighbours that philosopher returned forks
  check(left);
  check(right);

  pthread_mutex_unlock(&mutex);
}

void check(int tid) {
  int left = (tid + num_philosophers - 1) % num_philosophers;
  int right = (tid + 1) % num_philosophers;

  bool is_valid = (get_philosopher_status(tid) == HUNGRY &&
                   get_philosopher_status(left) != EATING &&
                   get_philosopher_status(right) != EATING);

  if (is_valid) {
    set_philosopher_status(tid, EATING);
    update_sub_window(tid);
    sleep(2);
    pthread_cond_signal(conditions[tid]);
  }
}