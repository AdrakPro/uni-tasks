#include "philosopher.h"

#define MAX_CAPACITY 10

Philosopher philosophers[MAX_CAPACITY];

void create_philosophers(WINDOW** windows, int philosophers_number) {
  for (int i = 0; i < philosophers_number; ++i) {
    Philosopher newPhilosopher;

    newPhilosopher.id = i;
    newPhilosopher.status = THINKING;
    newPhilosopher.window = windows[i];
    philosophers[i] = newPhilosopher;
  }
}

Philosopher* get_philosopher(int id) {
  return &philosophers[id];
}

void set_philosopher_status(int id, enum PhilosopherStatus status) {
  get_philosopher(id)->status = status;
}

enum PhilosopherStatus get_philosopher_status(int id) {
  return get_philosopher(id)->status;
}

void set_forks(int id, int left, int right) {
  get_philosopher(id)->forks[0] = left;
  get_philosopher(id)->forks[1] = right;
}
