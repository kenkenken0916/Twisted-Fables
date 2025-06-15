#include "tui.h"
#include <ncurses.h>
#include <signal.h>
#include <string.h>
#include "architecture.h"

volatile sig_atomic_t running = 1;

static void handle_sigint(int sig) {
    running = 0;
}

short get_card_color(int id) {
    if (id < 10) {
        int section = (id - 1) / 3;
        if (section == 0) return COLOR_YELLOW;
        if (section == 1) return COLOR_BLUE;
        if (section == 2) return COLOR_RED;
    } else if (id == 10) {
        return COLOR_YELLOW;
    } else if (id <= 130) {
        int block = (id - 10) / 10;
        if (block < 10) {
            int type = block % 3;
            if (type == 0) return COLOR_YELLOW;
            if (type == 1) return COLOR_BLUE;
            if (type == 2) return COLOR_RED;
        } else {
            return COLOR_MAGENTA; // rainbow
        }
    } else if (id == 131 || id == 134) {
        return COLOR_YELLOW;
    } else if (id == 132) {
        return COLOR_BLUE;
    } else if (id == 133) {
        return COLOR_RED;
    }
    return COLOR_WHITE;
}

int tui_init(TUI *tui) {
    signal(SIGINT, handle_sigint);
    initscr();
    noecho();
    cbreak();
    curs_set(0);

    start_color();
    use_default_colors();
    init_pair(1, COLOR_YELLOW, -1);
    init_pair(2, COLOR_BLUE, -1);
    init_pair(3, COLOR_RED, -1);
    init_pair(4, COLOR_WHITE, -1);
    init_pair(5, COLOR_GREEN, -1);
    init_pair(6, COLOR_MAGENTA, -1);

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

void draw_player_info(player *p, WINDOW *win, int row, int col) {
    mvwprintw(win, row, col, "Character: %d (Team %d)", p->character, p->team);
    mvwprintw(win, row + 1, col, "HP: %d/%d  DEF: %d/%d  EN: %d",
              p->life, p->maxlife, p->defense, p->maxdefense, p->energy);

    mvwprintw(win, row + 2, col, "Hand (%d):", p->hand.size);

    int max_hand = p->hand.size > 12 ? 12 : p->hand.size;

    for (int i = 0; i < max_hand; ++i) {
        card *c = (card *)vector_get(&p->hand, i);
        int cid = c ? c->id : -1;
        short color = get_card_color(cid);

        short pair = (color == COLOR_YELLOW) ? 1 :
                     (color == COLOR_BLUE) ? 2 :
                     (color == COLOR_RED) ? 3 :
                     (color == COLOR_GREEN) ? 5 :
                     (color == COLOR_MAGENTA) ? 6 : 4;

        wattron(win, COLOR_PAIR(pair));
        mvwprintw(win, row + 3 + i, col, "- [%s]", (c && c->name) ? c->name : "?");
        wattroff(win, COLOR_PAIR(pair));
    }
}

void draw_game_screen(game *gameState) {
    TUI *tui = gameState->tui;

    werase(tui->event_win);
    box(tui->event_win, 0, 0);
    for (int i = 0; i < gameState->log_size && i + 1 < tui->height - 1; ++i) {
        mvwprintw(tui->event_win, i + 1, 1, "%s", gameState->logs[i]);
    }
    wrefresh(tui->event_win);

    draw_battlefield(gameState);

    werase(tui->stat_win);
    box(tui->stat_win, 0, 0);

    int half_width = COLS / 2;

    mvwprintw(tui->stat_win, 0, 2, "Player 1");
    mvwprintw(tui->stat_win, 0, half_width + 2, "Player 2");

    for (int i = 1; i < LINES - 2 * tui->height - 1; ++i) {
        mvwprintw(tui->stat_win, i, half_width, "|");
    }

    draw_player_info(&gameState->player1, tui->stat_win, 1, 2);
    draw_player_info(&gameState->player2, tui->stat_win, 1, half_width + 2);

    wrefresh(tui->stat_win);
}
