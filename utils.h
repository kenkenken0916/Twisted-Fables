#ifndef _UTILS_H
#define _UTILS_H

#include "architecture.h"

// 抽牌函數
bool draw_card(player* player, int count);

// 移動檢查
bool can_move_to(game* gameState, int player_id, int x, int y);

// 距離計算
int calculate_distance(int x1, int y1, int x2, int y2);

// 範圍檢查
bool is_in_range(game* gameState, int attacker_id, int target_id, int range);

// 能量檢查
bool has_enough_energy(player* player, int cost);

// 生命值和防禦值調整
void adjust_life(player* player, int amount);
void adjust_defense(player* player, int amount);

// 牌組操作
void move_card(vector* from, vector* to, int index);
void move_all_cards(vector* from, vector* to);

// 狀態檢查
bool can_perform_action(game* gameState, int action_type);

#endif // _UTILS_H
