#include "tui.h"
#include <ncurses.h>
#include <signal.h>
#include <string.h>
#include "architecture.h"

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

// 繪製戰場
void draw_battlefield(game *gameState) {
    TUI *tui = gameState->tui;
    WINDOW *win = tui->pos_win;
    int width = gameState->map_width;

    werase(win);
    box(win, 0, 0);

    for (int i = 0; i < width; ++i) {
        mvwprintw(win, 1, 1 + i * 2, "%d", i);
    }

    for (int i = 0; i < width; ++i) {
        char c = ' ';
        if (i == gameState->player1.locate[0] && i == gameState->player2.locate[0])
            c = 'X';
        else if (i == gameState->player1.locate[0])
            c = '1';
        else if (i == gameState->player2.locate[0])
            c = '2';

        mvwprintw(win, 2, 1 + i * 2, "%c", c);
    }

    wrefresh(win);
}

// 顯示玩家資訊與手牌
void draw_player_info(player *p, WINDOW *win, int row) {
    mvwprintw(win, row, 1, "Character: %d (Team %d)", p->character, p->team);
    mvwprintw(win, row + 1, 1, "HP: %d/%d  DEF: %d/%d  EN: %d", 
              p->life, p->maxlife, p->defense, p->maxdefense, p->energy);

    mvwprintw(win, row + 2, 1, "Hand: ");
    int show_count = p->hand.size < 5 ? p->hand.size : 5;
    for (int i = 0; i < show_count; ++i) {
        card *c = (card *)vector_get(&p->hand, i);
        if (c && c->name)
            wprintw(win, "[%s] ", c->name);
        else
            wprintw(win, "[?] ");
    }
}

// 繪製整體畫面
void draw_game_screen(game *gameState) {
    TUI *tui = gameState->tui;

    // 畫事件紀錄
    werase(tui->event_win);
    box(tui->event_win, 0, 0);
    for (int i = 0; i < gameState->log_size; ++i) {
        mvwprintw(tui->event_win, i + 1, 1, "%s", gameState->logs[i]);
    }
    wrefresh(tui->event_win);

    // 畫戰場
    draw_battlefield(gameState);

    // 畫玩家狀態與手牌
    werase(tui->stat_win);
    box(tui->stat_win, 0, 0);
    draw_player_info(&gameState->player1, tui->stat_win, 1);
    draw_player_info(&gameState->player2, tui->stat_win, 7);
    wrefresh(tui->stat_win);
}
