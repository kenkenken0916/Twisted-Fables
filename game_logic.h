#ifndef _GAME_LOGIC_H
#define _GAME_LOGIC_H

#include "architecture.h"
#include "card_system.h"  // 添加卡牌系統頭文件
#include "game_state.h"   // 添加遊戲狀態頭文件

// 顯示遊戲狀態
void display_game_state(game* gameState);

// 處理玩家選擇
bool handle_player_choice(game* gameState, int32_t choice);

// 檢查遊戲是否結束
bool check_game_end(game* gameState);

// 顯示遊戲結果
void display_game_result(game* gameState);

// 處理玩家回合
void handle_player_turn(game* gameState);

// 處理不同的遊戲狀態
void handle_choose_identity(game* gameState, int8_t choice);
void handle_choose_move(game* gameState, int32_t choice);
void handle_use_card(game* gameState, int32_t cardIndex);
bool handle_buy_card(game* gameState, int32_t cardType);

#endif // _GAME_LOGIC_H
