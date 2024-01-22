#include <pthread.h>
#include <stdlib.h>

#include "utils.h"
#include "text_interface.h"

int philosophers_number;
bool is_sim_running = true;
pthread_mutex_t mutex;
pthread_cond_t** conditions;

void* start_sim(void* arg);

void* refresh_windows();

void grab_forks(int id);

void return_forks(int id);

int left(int id);

int right(int id);

int can_i_eat(int id);

void display_result();

int main(int argc, char* argv[]) {
  // Initialize philosophers_number and validate it
  if (argc != 2) {
    printf("The number of philosophers is missing!\n");
    return EXIT_FAILURE;
  }

  philosophers_number = atoi(argv[1]);

  if (!is_number_in_range(philosophers_number)) {
    printf("The number must be in range from 5 to 10!\n");
    return EXIT_FAILURE;
  }

  // Init Ncurses
  init_ncurses();
  WINDOW** windows = (WINDOW**) malloc(philosophers_number * sizeof(WINDOW*));
  draw_philosophers(windows, philosophers_number);

  // Init mutex
  pthread_mutex_init(&mutex, NULL);

  // Fill array with philosophers
  create_philosophers(windows, philosophers_number);

  conditions = (pthread_cond_t**) malloc(philosophers_number * sizeof(pthread_cond_t*));

  for (int i = 0; i < philosophers_number; i++) {
    conditions[i] = (pthread_cond_t*) malloc(sizeof(pthread_cond_t));
    pthread_cond_init(conditions[i], NULL);
  }

  pthread_t window_update_thread;
  pthread_t threads[philosophers_number];
  int ids[philosophers_number];

  for (int i = 0; i < philosophers_number; i++) {
    ids[i] = i;
    pthread_create(&threads[i], NULL, start_sim, (void*) &ids[i]);
  }

  pthread_create(&window_update_thread, NULL, refresh_windows, NULL);

  // With for loop main thread is waiting sequentially for end of the threads preventing race condition
  for (int i = 0; i < philosophers_number; i++) {
    pthread_join(threads[i], NULL);
  }
  is_sim_running = false;
  pthread_join(window_update_thread, NULL);

  // Cleanup
  pthread_mutex_destroy(&mutex);
  for (int i = 0; i < philosophers_number; i++) {
    pthread_cond_destroy(conditions[i]);
  }
  exit_windows(windows, philosophers_number);
  free(conditions);
  free(windows);
  exit_ncurses();

  display_result();

  return EXIT_SUCCESS;
}

void* start_sim(void* arg) {
  while (is_sim_running) {
    int id = *(int*) arg;
    sleep(1);
    grab_forks(id);
    sleep(1);
    return_forks(id);
  }

  return NULL;
}

// When a philosopher wants to eat, he checks both chopsticks
// If they are free, then he eats. Otherwise, he waits on a condition variable
// Whenever a philosopher finishes eating, he checks to see if his neighbors want to eat and are waiting
// Then he calls signal on their condition variables so that they can recheck the chopsticks and eat if possible
void grab_forks(int id) {
  pthread_mutex_lock(&mutex);

  set_status(id, HUNGRY);
  increment_ticket(id);

  // If true, release lock on mutex and put to sleep until another thread signal condition variable
  while (!can_i_eat(id)) {
    pthread_cond_wait(conditions[id], &mutex);
  }

  set_status(id, EATING);
  set_forks(id, left(id) + 1);
  increment_eat_count(id);
  sleep(2);

  pthread_mutex_unlock(&mutex);
}

void return_forks(int id) {
  pthread_mutex_lock(&mutex);

  set_status(id, THINKING);

  // Signal neighbours that philosopher returned forks
  pthread_cond_signal(conditions[left(id)]);
  pthread_cond_signal(conditions[right(id)]);

  pthread_mutex_unlock(&mutex);
}

int can_i_eat(int id) {
  // Take forks if both are available and if it has the lowest ticket than its neighbours
  if (get_status(left(id)) == EATING || get_status(right(id)) == EATING) return 0;
  if (get_status(left(id)) == HUNGRY && get_ticket(left(id)) < get_ticket(id)) return 0;
  if (get_status(right(id)) == HUNGRY && get_ticket(right(id)) < get_ticket(id)) return 0;

  return 1;
}

int left(int id) {
  return (id + philosophers_number - 1) % philosophers_number;
}

int right(int id) {
  return (id + 1) % philosophers_number;
}

void* refresh_windows() {
  while (is_sim_running) {
    if (getch() != ERR) {
      is_sim_running = false;
    }

    for (int i = 0; i < philosophers_number; ++i) {
      update_window(i);
    }
    // Sleep for 0.3 second
    usleep(300000);
  }

  return NULL;
}

void display_result() {
  for (int i = 0; i < philosophers_number; ++i) {
    printf("Philosopher %d has eaten %d times.\n", i + 1, get_eat_count(i));
  }
}