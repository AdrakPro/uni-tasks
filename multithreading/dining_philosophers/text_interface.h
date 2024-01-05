#ifndef DINING_PHILOSOPHERS_TEXT_INTERFACE_H
#define DINING_PHILOSOPHERS_TEXT_INTERFACE_H

#include "philosopher.h"

void initNcurses();
WINDOW** drawPhilosophers(int windows_number);
void updateSubWindow(Philosopher* philosopher);
void exitSubWindows(WINDOW** windows, int windows_number);
void exitNcurses();


#endif //DINING_PHILOSOPHERS_TEXT_INTERFACE_H
