#ifndef DINING_PHILOSOPHERS_TEXT_INTERFACE_H
#define DINING_PHILOSOPHERS_TEXT_INTERFACE_H

#include <unistd.h>
#include "philosopher.h"

void init_ncurses();

void draw_philosophers(WINDOW** windows, int windows_number);

void update_window(int id);

void exit_windows(WINDOW** windows, int windows_number);

void exit_ncurses();


#endif //DINING_PHILOSOPHERS_TEXT_INTERFACE_H
