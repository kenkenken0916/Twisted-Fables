#ifndef TUI_H
#define TUI_H

#include <ncurses.h>

#define WIDTH 9
#define DEF_CEIL 6

typedef struct {
    int pos;
    int hp;
    int mp;
    int def;
} Player;

typedef struct {
    WINDOW *event_win;
    WINDOW *pos_win;
    WINDOW *stat_win;
    int height;
} TUI;

// TUI initialization and cleanup
int tui_init(TUI *tui);
void tui_cleanup(TUI *tui);

// Drawing functions
void draw_event_log(WINDOW *win, const char *log[], int log_size);
void draw_positions(WINDOW *win, Player p1, Player p2);
void draw_stats(WINDOW *win, Player p1, Player p2);

// Main update function
void tui_update(TUI *tui, Player p1, Player p2, const char *logs[], int log_size);

#endif // TUI_H
