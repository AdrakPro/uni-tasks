#include <ncurses.h>
#include <stdlib.h>
#include "text_interface.h"

void initNcurses() {
  initscr();
  cbreak();
  noecho();
  curs_set(0);
  timeout(0);
}

WINDOW** drawPhilosophers(int windows_number) {
  int boxWidth = 30;
  int startX = 1;
  int currentX = startX;
  int currentY = 1;
  int TERMINAL_WIDTH = getmaxx(stdscr);

  WINDOW** windows = (WINDOW**) calloc(windows_number, sizeof(WINDOW*));

  for (int i = 0; i < windows_number; ++i) {
    refresh();

    // Check if the next box exceeds the terminal width
    if (currentX + boxWidth > TERMINAL_WIDTH) {
      currentX = startX;
      currentY += 6;
    }

    windows[i] = newwin(5, boxWidth, currentY, currentX);
    box(windows[i], 0, 0);
    mvwprintw(windows[i], 1, 2, "Philosopher %d", i + 1);
    mvwprintw(windows[i], 3, 2, "Status: %s", "Thinking");
    wrefresh(windows[i]);
    // Move to the next column
    currentX += boxWidth + 2;
  }

  return windows;
}

void updateSubWindow(Philosopher* philosopher) {
  werase(philosopher->window);
  box(philosopher->window, 0, 0);

  mvwprintw(philosopher->window, 1, 2, "Philosopher %d", philosopher->id + 1);
  mvwprintw(philosopher->window, 3, 2, "Status: ");
  if (philosopher->status == THINKING) {
    wprintw(philosopher->window, "Thinking");
  } else if (philosopher->status == HUNGRY) {
    wprintw(philosopher->window, "Hungry");
  } else if (philosopher->status == EATING) {
    wprintw(philosopher->window, "Eating");
  }

  wrefresh(philosopher->window);
  refresh();
}

void exitSubWindows(WINDOW** sub_windows, int sub_windows_number) {
  for (int i = 0; i < sub_windows_number; ++i) {
    delwin(sub_windows[i]);
  }
  free(sub_windows);
}

void exitNcurses() {
  endwin();
}
