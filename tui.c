#include "tui.h"
#include <ncurses.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>

volatile sig_atomic_t running = 1;

void handle_sigint(int sig) {
    running = 0;
}

int tui_init(TUI *tui) {
    signal(SIGINT, handle_sigint);
    
    initscr();
    noecho();
    cbreak();
    curs_set(0);

    tui->height = LINES / 3;

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

void draw_event_log(WINDOW *win, const char *log[], int log_size) {
    werase(win);  // 完全清除視窗
    box(win, 0, 0);  // 重新繪製邊框
    
    // 重新繪製所有日誌內容
    for (int i = 0; i < log_size; ++i) {
        mvwprintw(win, i + 1, 1, "%s", log[i]);
    }
    
    wrefresh(win);  // 強制重新繪製到螢幕
}

void draw_positions(WINDOW *win, Player p1, Player p2) {
    werase(win);  // 完全清除視窗
    box(win, 0, 0);  // 重新繪製邊框
    
    // 重新繪製位置標籤
    mvwprintw(win, 1, 1, "0 1 2 3 4 5 6 7 8");

    // 重新繪製所有位置
    for (int i = 0; i < WIDTH; ++i) {
        char c = ' ';
        if (i == p1.pos && i == p2.pos)
            c = 'X'; // 兩個玩家在同一格
        else if (i == p1.pos)
            c = '1';
        else if (i == p2.pos)
            c = '2';

        mvwprintw(win, 2, 1 + i * 2, "%c", c);
    }
    
    wrefresh(win);  // 強制重新繪製到螢幕
}

void draw_stats(WINDOW *win, Player p1, Player p2) {
    werase(win);  // 完全清除視窗
    box(win, 0, 0);  // 重新繪製邊框
    
    // 重新繪製玩家狀態
    mvwprintw(win, 1, 1, "P1 HP: %2d  MP: %d  DEF: %d", p1.hp, p1.mp, p1.def);
    mvwprintw(win, 2, 1, "P2 HP: %2d  MP: %d  DEF: %d", p2.hp, p2.mp, p2.def);
    
    wrefresh(win);  // 強制重新繪製到螢幕
}

void tui_update(TUI *tui, Player p1, Player p2, const char *logs[], int log_size) {
    // 每次調用都完全重新繪製所有視窗
    draw_event_log(tui->event_win, logs, log_size);
    draw_positions(tui->pos_win, p1, p2);
    draw_stats(tui->stat_win, p1, p2);
    
    // 確保所有變更都顯示到螢幕上
    refresh();
}

// Example main function showing how to use the TUI
int main() {
    TUI tui = {0};
    
    if (tui_init(&tui) != 0) {
        fprintf(stderr, "Failed to initialize TUI\n");
        return 1;
    }

    Player p1 = {3, 36, 0, 0};
    Player p2 = {5, 36, 0, 0};

    const char *logs[] = {
        ". . . . . . . . .",
        ". . . . . . . . ."
    };

    while (running) {
        tui_update(&tui, p1, p2, logs, 2);
        usleep(100000);
    }

    tui_cleanup(&tui);
    return 0;
}
