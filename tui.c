#include "tui.h"
#include <ncurses.h>
#include <signal.h>
#include <unistd.h>

volatile sig_atomic_t running = 1;

static void handle_sigint(int sig) {
    running = 0;
}

int tui_init(TUI *tui) {
    signal(SIGINT, handle_sigint);
    initscr();
    noecho();
    cbreak();
    curs_set(0);

    tui->height = LINES / DEFAULT_HEIGHT_DIVIDER;
    tui->event_win = newwin(tui->height, COLS, 0, 0);
    tui->pos_win   = newwin(tui->height, COLS, tui->height, 0);
    tui->stat_win  = newwin(LINES - 2 * tui->height, COLS, 2 * tui->height, 0);

    if (!tui->event_win || !tui->pos_win || !tui->stat_win) {
        tui_cleanup(tui);
        return -1;
    }
    return 0;
}

void tui_cleanup(TUI *tui) {
    if (tui->event_win) {
        delwin(tui->event_win);
        tui->event_win = NULL;
    }
    if (tui->pos_win) {
        delwin(tui->pos_win);
        tui->pos_win = NULL;
    }
    if (tui->stat_win) {
        delwin(tui->stat_win);
        tui->stat_win = NULL;
    }
    endwin();
}

void tui_draw_event_log(WINDOW *win, const char *log[], int log_size) {
    werase(win);
    box(win, 0, 0);
    for (int i = 0; i < log_size; ++i) {
        mvwprintw(win, i + 1, 1, "%s", log[i]);
    }
    wrefresh(win);
}

void tui_draw_positions(WINDOW *win, int width, int p1_pos, int p2_pos) {
    werase(win);
    box(win, 0, 0);
    
    // 顯示位置標題 (例如："0 1 2 3 4 5 6 7 8")
    for (int i = 0; i < width; ++i) {
        mvwprintw(win, 1, 1 + i * 2, "%d", i);
    }
    
    // 在第二列標記出玩家位置
    for (int i = 0; i < width; ++i) {
        char c = ' ';
        if (i == p1_pos && i == p2_pos)
            c = 'X';   // 兩個玩家同時在此位置
        else if (i == p1_pos)
            c = '1';
        else if (i == p2_pos)
            c = '2';
        
        mvwprintw(win, 2, 1 + i * 2, "%c", c);
    }
    wrefresh(win);
}

void tui_draw_stats(WINDOW *win, int p1_hp, int p1_mp, int p1_def,
                      int p2_hp, int p2_mp, int p2_def) {
    werase(win);
    box(win, 0, 0);
    mvwprintw(win, 1, 1, "P1 HP: %2d  MP: %d  DEF: %d", p1_hp, p1_mp, p1_def);
    mvwprintw(win, 2, 1, "P2 HP: %2d  MP: %d  DEF: %d", p2_hp, p2_mp, p2_def);
    wrefresh(win);
}

void tui_update(TUI *tui, int width,
                int p1_pos, int p2_pos,
                int p1_hp, int p1_mp, int p1_def,
                int p2_hp, int p2_mp, int p2_def,
                const char *logs[], int log_size) {
    tui_draw_event_log(tui->event_win, logs, log_size);
    tui_draw_positions(tui->pos_win, width, p1_pos, p2_pos);
    tui_draw_stats(tui->stat_win, p1_hp, p1_mp, p1_def, p2_hp, p2_mp, p2_def);
}
