#include "tui.h"
#include <string.h>
#include <ncurses.h>
#include <unistd.h>
#include <signal.h>
#include "vector.h"
#include "architecture.h"

volatile sig_atomic_t running = 1;

static TUI g_tui;
static vector g_event_logs;
static bool g_tui_initialized = false;

#define MAX_LOG_LENGTH 256
#define MAX_LOGS 100

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
            return COLOR_MAGENTA;
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

int tui_init() {
    if (g_tui_initialized) {
        return 0;
    }

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

    g_tui.height = LINES / DEFAULT_HEIGHT_DIVIDER;
    g_tui.event_win = newwin(g_tui.height, COLS, 0, 0);
    g_tui.pos_win   = newwin(g_tui.height, COLS, g_tui.height, 0);
    g_tui.stat_win  = newwin(LINES - 2 * g_tui.height, COLS, 2 * g_tui.height, 0);

    if (!g_tui.event_win || !g_tui.pos_win || !g_tui.stat_win) {
        tui_cleanup();
        return -1;
    }

    g_event_logs = initVector();
    g_tui_initialized = true;
    
    return 0;
}

void tui_cleanup() {
    if (g_tui.event_win) {
        delwin(g_tui.event_win);
        g_tui.event_win = NULL;
    }
    if (g_tui.pos_win) {
        delwin(g_tui.pos_win);
        g_tui.pos_win = NULL;
    }
    if (g_tui.stat_win) {
        delwin(g_tui.stat_win);
        g_tui.stat_win = NULL;
    }
    
    clearVector(&g_event_logs);
    g_tui_initialized = false;
    
    endwin();
}

void tui_add_log(const char* message) {
    if (!g_tui_initialized || !message) return;
    
    if (g_event_logs.SIZE >= MAX_LOGS) {
        eraseVector(&g_event_logs, 0);
    }
    
    char* log_entry = malloc(MAX_LOG_LENGTH);
    if (log_entry) {
        strncpy(log_entry, message, MAX_LOG_LENGTH - 1);
        log_entry[MAX_LOG_LENGTH - 1] = '\0';
        pushbackVector(&g_event_logs, (int32_t)(intptr_t)log_entry);
    }
}

void tui_clear_logs() {
    if (!g_tui_initialized) return;
    
    for (int i = 0; i < g_event_logs.SIZE; i++) {
        int32_t success;
        char* log_entry = (char*)(intptr_t)getVector(&g_event_logs, i, &success);
        if (success && log_entry) {
            free(log_entry);
        }
    }
    clearVector(&g_event_logs);
}

void draw_battlefield(player* players, int battlefield_width) {
    if (!g_tui_initialized || !players) return;
    
    WINDOW *win = g_tui.pos_win;
    int width = battlefield_width > 0 ? battlefield_width : 10; // 默认宽度

    werase(win);
    box(win, 0, 0);
    
    mvwprintw(win, 0, 2, "Battlefield");

    for (int i = 0; i < width; ++i) {
        mvwprintw(win, 1, 1 + i * 2, "%d", i);
    }

    for (int i = 0; i < width; ++i) {
        char c = ' ';
        if (i == players[0].locate[0] && i == players[1].locate[0])
            c = 'X';
        else if (i == players[0].locate[0])
            c = '1';
        else if (i == players[1].locate[0])
            c = '2';

        mvwprintw(win, 2, 1 + i * 2, "%c", c);
    }

    wrefresh(win);
}

void draw_player_info(player *p, WINDOW *win, int row, int col, int player_id) {
    if (!p || !win) return;
    
    mvwprintw(win, row, col, "Player %d - Character: %d (Team %d)", 
              player_id, p->character, p->team);
    mvwprintw(win, row + 1, col, "HP: %d/%d  DEF: %d/%d  EN: %d",
              p->life, p->maxlife, p->defense, p->maxdefense, p->energy);

    mvwprintw(win, row + 2, col, "- Life: %d", p->life);
    mvwprintw(win, row + 3, col, "- Defense: %d", p->defense);
    mvwprintw(win, row + 4, col, "- Energy: %d", p->energy);

    mvwprintw(win, row + 5, col, "- Metamorphosis:");
    int meta_row = row + 6;
    
    for (int i = 0; i < p->metamorphosis.SIZE && meta_row < LINES - 2 * g_tui.height - 2; ++i) {
        int32_t success;
        int32_t card_ptr = getVector(&p->metamorphosis, i, &success);
        if (success && card_ptr != 0) {
            card *c = (card *)(intptr_t)card_ptr;
            if (c && c->id <= 135) {
                short color = get_card_color(c->id);
                short pair = (color == COLOR_YELLOW) ? 1 :
                           (color == COLOR_BLUE) ? 2 :
                           (color == COLOR_RED) ? 3 :
                           (color == COLOR_GREEN) ? 5 :
                           (color == COLOR_MAGENTA) ? 6 : 4;

                wattron(win, COLOR_PAIR(pair));
                mvwprintw(win, meta_row++, col + 2, "- [%s]", c->name ? c->name : "?");
                wattroff(win, COLOR_PAIR(pair));
            }
        }
    }

    meta_row += 1;
    mvwprintw(win, meta_row, col, "Hand (%d):", p->hand.SIZE);

    int max_hand = p->hand.SIZE > 12 ? 12 : p->hand.SIZE;
    for (int i = 0; i < max_hand && meta_row + 1 + i < LINES - 2 * g_tui.height - 2; ++i) {
        int32_t success;
        int32_t card_ptr = getVector(&p->hand, i, &success);
        if (success && card_ptr != 0) {
            card *c = (card *)(intptr_t)card_ptr;
            int cid = c ? c->id : -1;
            short color = get_card_color(cid);

            short pair = (color == COLOR_YELLOW) ? 1 :
                        (color == COLOR_BLUE) ? 2 :
                        (color == COLOR_RED) ? 3 :
                        (color == COLOR_GREEN) ? 5 :
                        (color == COLOR_MAGENTA) ? 6 : 4;

            wattron(win, COLOR_PAIR(pair));
            mvwprintw(win, meta_row + 1 + i, col, "- [%s]", (c && c->name) ? c->name : "Unknown");
            wattroff(win, COLOR_PAIR(pair));
        }
    }
}

void draw_event_logs() {
    if (!g_tui_initialized) return;
    
    werase(g_tui.event_win);
    box(g_tui.event_win, 0, 0);
    mvwprintw(g_tui.event_win, 0, 2, "Events");
    
    int max_logs = g_tui.height - 2;
    int start_index = g_event_logs.SIZE > max_logs ? g_event_logs.SIZE - max_logs : 0;
    
    for (int i = start_index; i < g_event_logs.SIZE; ++i) {
        int32_t success;
        char* log_entry = (char*)(intptr_t)getVector(&g_event_logs, i, &success);
        if (success && log_entry) {
            mvwprintw(g_tui.event_win, i - start_index + 1, 1, "%s", log_entry);
        }
    }
    
    wrefresh(g_tui.event_win);
}

void draw_game_screen(player* players, int battlefield_width) {
    if (!g_tui_initialized || !players) return;

    draw_event_logs();

    draw_battlefield(players, battlefield_width);

    werase(g_tui.stat_win);
    box(g_tui.stat_win, 0, 0);

    int half_width = COLS / 2;

    mvwprintw(g_tui.stat_win, 0, 2, "Player 1");
    mvwprintw(g_tui.stat_win, 0, half_width + 2, "Player 2");

    for (int i = 1; i < LINES - 2 * g_tui.height - 1; ++i) {
        mvwprintw(g_tui.stat_win, i, half_width, "|");
    }

    draw_player_info(&players[0], g_tui.stat_win, 1, 2, 1);
    draw_player_info(&players[1], g_tui.stat_win, 1, half_width + 2, 2);

    wrefresh(g_tui.stat_win);
}

void tui_refresh() {
    if (!g_tui_initialized) return;
    
    refresh();
    if (g_tui.event_win) wrefresh(g_tui.event_win);
    if (g_tui.pos_win) wrefresh(g_tui.pos_win);
    if (g_tui.stat_win) wrefresh(g_tui.stat_win);
}

bool tui_is_initialized() {
    return g_tui_initialized;
}

TUI* tui_get_instance() {
    return g_tui_initialized ? &g_tui : NULL;
}
