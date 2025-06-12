#ifndef TUI_H
#define TUI_H

#include <ncurses.h>
#include "game.h"

#define DEFAULT_HEIGHT_DIVIDER 6

typedef struct {
    WINDOW *event_win;
    WINDOW *pos_win;
    WINDOW *stat_win;
    int height;
} TUI;

int tui_init(TUI *tui);
void tui_cleanup(TUI *tui);

void draw_game_screen(game *gameState);      // 繪製遊戲畫面
void draw_battlefield(game *gameState);      // 繪製戰場
void draw_player_info(player *p, WINDOW *win, int row);  // 顯示玩家資訊

#endif
