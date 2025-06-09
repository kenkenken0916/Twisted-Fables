#ifndef GAME_STATE_H
#define GAME_STATE_H

#include "architecture.h"

// 遊戲狀態相關函數
void transition_to_next_state(game* gameState);
void handle_start_phase(game* gameState);
void handle_cleanup_phase(game* gameState);
void handle_action_phase(game* gameState);
void handle_focus_action(game* gameState);
void handle_attack_action(game* gameState);
void start_turn(game* gameState);
void end_turn(game* gameState);
void apply_damage(game* gameState, int target_player, int damage);
void apply_defense(game* gameState, int target_player, int defense);
bool check_attack_range(game* gameState, int attacker, int target);

#endif /* GAME_STATE_H */
