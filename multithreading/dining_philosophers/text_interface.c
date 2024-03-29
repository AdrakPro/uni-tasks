#include "text_interface.h"

void change_text_color(WINDOW* window, char* text, int pair_index);

void init_ncurses() {
  initscr();

  start_color();
  init_pair(1, COLOR_WHITE, COLOR_BLACK);
  init_pair(2, COLOR_RED, COLOR_BLACK);
  init_pair(3, COLOR_GREEN, COLOR_BLACK);

  cbreak(); // Take input without clicking enter
  noecho(); // The characters typed by the user will not be displayed on the screen.
  curs_set(0); // Hide cursor
  nodelay(stdscr, TRUE); // getch() will not block, it will return ERR if no input is waiting
}

void draw_philosophers(WINDOW** windows, int windows_number) {
  int box_width = 36;
  int start_x = 1;
  int current_x = start_x;
  int current_y = 1;
  int terminal_width = getmaxx(stdscr);

  for (int i = 0; i < windows_number; ++i) {
    // Check if the next box exceeds the terminal width
    if (current_x + box_width > terminal_width) {
      current_x = start_x;
      current_y += 8;
    }

    windows[i] = newwin(7, box_width, current_y, current_x);
    box(windows[i], 0, 0);
    mvwprintw(windows[i], 1, 2, "Philosopher %d", i + 1);
    mvwprintw(windows[i], 3, 2, "Status: ");
    change_text_color(windows[i], "Thinking", 1);
    wrefresh(windows[i]);
    // Move window to the next row
    current_x += box_width + 2;
  }
}

void update_window(int id) {
  Philosopher* philosopher = get_philosopher(id);
  WINDOW* window = philosopher->window;
  enum PhilosopherStatus status = philosopher->status;

  werase(window);
  box(window, 0, 0);

  mvwprintw(window, 1, 2, "Philosopher %d", id + 1);
  mvwprintw(window, 3, 2, "Status: ");
  if (status == THINKING) {
    change_text_color(window, "Thinking ", 1);
  } else if (status == HUNGRY) {
    change_text_color(window, "Hungry", 2);
  } else if (status == EATING) {
    int left_fork = philosopher->forks[0];
    int right_fork = philosopher->forks[1];

    change_text_color(window, "Eating ", 3);
    mvwprintw(window, 5, 2, "Forks taken: %d, %d", left_fork, right_fork);
  }

  wrefresh(window);
}

void change_text_color(WINDOW* window, char* text, int pair_index) {
  wattron(window, COLOR_PAIR(pair_index) | A_BOLD);
  wprintw(window, "%s", text);
  wattroff(window, COLOR_PAIR(pair_index) | A_BOLD);
}

void exit_windows(WINDOW** windows, int windows_number) {
  for (int i = 0; i < windows_number; ++i) {
    delwin(windows[i]);
  }
}

void exit_ncurses() {
  endwin();
}
