#ifndef DINING_PHILOSOPHERS_PHILOSOPHER_H
#define DINING_PHILOSOPHERS_PHILOSOPHER_H

#include <ncurses.h>

enum PhilosopherStatus {
    THINKING,
    HUNGRY,
    EATING
};

typedef struct {
    int id;
    int forks[2];
    enum PhilosopherStatus status;
    WINDOW* window;
} Philosopher;

void create_philosophers(WINDOW** windows, int philosophers_number);

Philosopher* get_philosopher(int id);

void set_philosopher_status(int id, enum PhilosopherStatus status);

enum PhilosopherStatus get_philosopher_status(int id);

void set_forks(int id, int left, int right);

#endif //DINING_PHILOSOPHERS_PHILOSOPHER_H