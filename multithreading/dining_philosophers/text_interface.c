#include "text_interface.h"

void change_text_color(WINDOW* window, char* text, int pair_index);

long get_rss();

void init_ncurses() {
  initscr();

  start_color();
  init_pair(1, COLOR_WHITE, COLOR_BLACK);
  init_pair(2, COLOR_RED, COLOR_BLACK);
  init_pair(3, COLOR_GREEN, COLOR_BLACK);

  cbreak();
  noecho();
  curs_set(0);
  nodelay(stdscr, TRUE);
  timeout(0);
}

void draw_philosophers(WINDOW** windows, int windows_number) {
  int box_width = 36;
  int start_x = 1;
  int current_x = start_x;
  int current_y = 1;
  int terminal_width = getmaxx(stdscr);

  for (int i = 0; i < windows_number; ++i) {
    refresh();

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
    // RSS (resident set size) is the portion of process's memory that is held in RAM and used by OS
    // Threads are in the same process, so their RSS is the same
    mvwprintw(windows[i], 5, 2, "RSS: %ld KB", get_rss());
    wrefresh(windows[i]);
    // Move window to the next column
    current_x += box_width + 2;
  }
}

void update_sub_window(int id) {
  Philosopher* philosopher = get_philosopher(id);
  WINDOW* window = philosopher->window;
  enum PhilosopherStatus status = philosopher->status;

  werase(window);
  box(window, 0, 0);

  mvwprintw(window, 1, 2, "Philosopher %d", id + 1);
  mvwprintw(window, 3, 2, "Status: ");
  if (status == THINKING) {
    change_text_color(window, "Thinking", 1);
  } else if (status == HUNGRY) {
    change_text_color(window, "Hungry", 2);
  } else if (status == EATING) {
    change_text_color(window, "Eating", 3);
  }

  mvwprintw(window, 5, 2, "RSS: %ld KB", get_rss());

  wrefresh(window);
  refresh();
}

void change_text_color(WINDOW* window, char* text, int pair_index) {
  wattron(window, COLOR_PAIR(pair_index) | A_BOLD);
  wprintw(window, "%s", text);
  wattroff(window, COLOR_PAIR(pair_index) | A_BOLD);
}

long get_rss() {
  struct rusage usage;
  getrusage(RUSAGE_SELF, &usage);

  return usage.ru_maxrss;
}

void exit_sub_windows(WINDOW** sub_windows, int sub_windows_number) {
  for (int i = 0; i < sub_windows_number; ++i) {
    delwin(sub_windows[i]);
  }
}

void exit_ncurses() {
  endwin();
}
