#ifndef TUI_H
#define TUI_H

#include <ncurses.h>
#include <stdbool.h>
#include <stdint.h>
#include <signal.h>

#define DEFAULT_HEIGHT_DIVIDER 3

typedef struct {
    WINDOW *event_win;
    WINDOW *pos_win;
    WINDOW *stat_win;
    int height;
} TUI;

typedef struct _card {
    int id;
    char *name;
} card;

typedef struct _player player;

extern volatile sig_atomic_t running;

// initialization and cleanup
int tui_init();
void tui_cleanup();

// log
void tui_add_log(const char* message);
void tui_clear_logs();

// functions to draw game elements
void draw_battlefield(player* players, int battlefield_width);
void draw_player_info(player *p, WINDOW *win, int row, int col, int player_id);
void draw_event_logs();
void draw_game_screen(player* players, int battlefield_width);

// tool functions
short get_card_color(int id);
void tui_refresh();
bool tui_is_initialized();
TUI* tui_get_instance();

#endif
