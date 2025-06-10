#include "constructure.h"

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
    werase(win);
    box(win, 0, 0);
    for (int i = 0; i < log_size; ++i) {
        mvwprintw(win, i + 1, 1, "%s", log[i]);
    }
    wrefresh(win);
}

void draw_positions(WINDOW *win, Player p1, Player p2) {
    werase(win);
    box(win, 0, 0);
    mvwprintw(win, 1, 1, "0 1 2 3 4 5 6 7 8");

    for (int i = 0; i < WIDTH; ++i) {
        char c = ' ';
        /* 這裡假設玩家的「位置」存放在 locate[0] */
        if (i == p1.locate[0] && i == p2.locate[0])
            c = 'X'; // 兩位玩家在同一格
        else if (i == p1.locate[0])
            c = '1';
        else if (i == p2.locate[0])
            c = '2';

        mvwprintw(win, 2, 1 + i * 2, "%c", c);
    }
    wrefresh(win);
}

void draw_stats(WINDOW *win, Player p1, Player p2) {
    werase(win);
    box(win, 0, 0);
    /* 這裡假設 p1.life 為生命值，p1.energy 為魔法點數，p1.defense 為防禦值 */
    mvwprintw(win, 1, 1, "P1 HP: %2d  MP: %d  DEF: %d", p1.life, p1.energy, p1.defense);
    mvwprintw(win, 2, 1, "P2 HP: %2d  MP: %d  DEF: %d", p2.life, p2.energy, p2.defense);
    wrefresh(win);
}

void tui_update(TUI *tui, Player p1, Player p2, const char *logs[], int log_size) {
    draw_event_log(tui->event_win, logs, log_size);
    draw_positions(tui->pos_win, p1, p2);
    draw_stats(tui->stat_win, p1, p2);
}

/* 範例 main 函式展示如何使用 TUI */
int main() {
    TUI tui = {0};
    
    if (tui_init(&tui) != 0) {
        fprintf(stderr, "Failed to initialize TUI\n");
        return 1;
    }

    Player p1 = {0};
    Player p2 = {0};
    /* 初始化玩家的資料，這裡設定位置、生命值等，根據需求調整 */
    p1.locate[0] = 3;
    p2.locate[0] = 5;
    p1.life = 36;
    p2.life = 36;
    p1.energy = 0;
    p2.energy = 0;
    p1.defense = 0;
    p2.defense = 0;

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
