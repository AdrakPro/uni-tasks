#include "philosopher.h"

#define MAX_CAPACITY 10

Philosopher philosophers[MAX_CAPACITY];

void createPhilosophers(WINDOW** windows, int philosophers_number) {
  for (int i = 0; i < philosophers_number; ++i) {
    Philosopher newPhilosopher;

    newPhilosopher.id = i;
    newPhilosopher.status = THINKING;
    newPhilosopher.window = windows[i];

    philosophers[i] = newPhilosopher;
  }
}

Philosopher* getPhilosopher(int id) {
  return &philosophers[id];
}

void setPhilosopherStatus(int id, enum PhilosopherStatus status) {
  getPhilosopher(id)->status = status;
}

enum PhilosopherStatus getPhilosopherStatus(int id) {
  return getPhilosopher(id)->status;
}
