#ifndef DINING_PHILOSOPHERS_PHILOSOPHER_H
#define DINING_PHILOSOPHERS_PHILOSOPHER_H

#include <ncurses.h>
#include <stdlib.h>

enum PhilosopherStatus {
    THINKING,
    HUNGRY,
    EATING
};

typedef struct {
    int id;
    WINDOW* window;
    enum PhilosopherStatus status;
} Philosopher;

void createPhilosophers(WINDOW** windows, int philosophers_number);
Philosopher* getPhilosopher(int id);
void setPhilosopherStatus(int id, enum PhilosopherStatus status);
enum PhilosopherStatus getPhilosopherStatus(int id);

#endif //DINING_PHILOSOPHERS_PHILOSOPHER_H