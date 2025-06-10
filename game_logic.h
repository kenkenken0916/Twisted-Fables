#ifndef _GAME_LOGIC_H
#define _GAME_LOGIC_H

#include "architecture.h"
#include "card_system.h" // 添加卡牌系統頭文件
#include "game_state.h"  // 添加遊戲狀態頭文件

// 顯示遊戲狀態
void display_game_state(game *gameState);

// 顯示可用卡牌
void display_available_cards(game *gameState, bool (*card_filter)(int32_t));

// 處理玩家選擇
bool handle_player_choice(game *gameState, int32_t choice);

// 處理玩家回合
void handle_player_turn(game *gameState);

// 處理不同的遊戲狀態
void handle_choose_identity(game *gameState, int8_t choice);
void handle_choose_move(game *gameState, int32_t choice);
void handle_use_card(game *gameState, int32_t cardIndex);
bool handle_buy_card(game *gameState, int32_t cardType);

// 檢查遊戲是否結束
bool check_game_end(game *gameState);

// 顯示遊戲結果
void display_game_result(game *gameState);

// 卡牌過濾函數
bool is_attack_card(int32_t cardId);
bool is_defense_card(int32_t cardId);
bool is_move_card(int32_t cardId);
bool is_skill_card(int32_t cardId);

#endif // _GAME_LOGIC_H
