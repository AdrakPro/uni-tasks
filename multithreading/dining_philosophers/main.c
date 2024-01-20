#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>

#include "utils.h"
#include "text_interface.h"

int num_philosophers;
pthread_mutex_t mutex;
pthread_cond_t** conditions;

void* start_sim(void* arg);

void grab_forks(int id);

void return_forks(int id);

void check(int id);

int left_fork(int id);

int right_fork(int id);

void display_eat_count();

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
  WINDOW** windows = (WINDOW**) malloc(num_philosophers * sizeof(WINDOW*));
  draw_philosophers(windows, num_philosophers);

  // Init mutex
  pthread_mutex_init(&mutex, NULL);

  // Fill the simple database with philosophers
  create_philosophers(windows, num_philosophers);

  // Dynamic allocation, because we are waiting for num_philosophers
  conditions = (pthread_cond_t**) malloc(num_philosophers * sizeof(pthread_cond_t*));

  for (int i = 0; i < num_philosophers; i++) {
    conditions[i] = (pthread_cond_t*) malloc(sizeof(pthread_cond_t));
    pthread_cond_init(conditions[i], NULL);
  }

  pthread_t threads[num_philosophers];

  int ids[num_philosophers];

  for (int i = 0; i < num_philosophers; i++) {
    ids[i] = i;
    pthread_create(&threads[i], NULL, start_sim, (void*) &ids[i]);
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
  free(windows);
  exit_ncurses();

  display_eat_count();

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

void grab_forks(int id) {
  pthread_mutex_lock(&mutex);

  set_philosopher_status(id, HUNGRY);
  update_sub_window(id);

  check(id);

  while (get_philosopher_status(id) != EATING) {
    pthread_cond_wait(conditions[id], &mutex);
  }

  pthread_mutex_unlock(&mutex);
  sleep(1);
}

void return_forks(int id) {
  pthread_mutex_lock(&mutex);

  set_philosopher_status(id, THINKING);
  update_sub_window(id);

  // Signal to neighbours that philosopher returned forks
  check(left_fork(id));
  check(right_fork(id));

  pthread_mutex_unlock(&mutex);
}

void check(int id) {
  // Take forks if both are available (deadlock prevention)
  bool is_valid = (get_philosopher_status(id) == HUNGRY &&
                   get_philosopher_status(left_fork(id)) != EATING &&
                   get_philosopher_status(right_fork(id)) != EATING);

  if (is_valid) {
    set_philosopher_status(id, EATING);
    set_forks(id, left_fork(id) + 1, id + 1);
    increment_eat_count(id);
    update_sub_window(id);
    sleep(2);
    pthread_cond_signal(conditions[id]);
  }
}

int left_fork(int id) {
  return (id + num_philosophers - 1) % num_philosophers;
}

int right_fork(int id) {
  return (id + 1) % num_philosophers;
}

void display_eat_count() {
  for (int i = 0; i < num_philosophers; ++i) {
    printf("Philosopher %d has eaten: %d times.\n", i + 1, get_eat_count(i));
  }
}