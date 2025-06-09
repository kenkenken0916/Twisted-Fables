#include <stdio.h>
#include <stdlib.h>
#include "game_state.h"
#include "debug_log.h"
#include "utils.h"
#include "game_logic.h"  // 添加此行以訪問 display_game_state

void transition_to_next_state(game* gameState) {
    player* current_player = &gameState->players[gameState->now_turn_player_id];
    
    switch (gameState->status) {
        case CHOOSE_IDENTITY:
            if (gameState->now_turn_player_id == 1) {
                gameState->status = CHOOSE_MOVE;
                gameState->now_turn_player_id = 0;
                initial_draw(gameState);
            } else {
                gameState->now_turn_player_id = 1;
            }
            break;
            
        case CHOOSE_MOVE:
            // 保持在當前狀態，等待玩家選擇行動
            break;
            
        case USE_ATK:
        case USE_DEF:
        case USE_MOV:
            if (current_player->hand.SIZE == 0 || gameState->nowUsingCardID == 0) {
                gameState->status = CHOOSE_MOVE;
            }
            break;
            
        case BUY_CARD_TYPE:
            gameState->status = CHOOSE_MOVE;
            break;
            
        // 其他狀態的轉換...
        
        default:
            break;
    }
}

void handle_start_phase(game* gameState) {
    DEBUG_LOG("開始階段");
    
    // 更新遊戲狀態
    gameState->status = CHOOSE_MOVE;
    
    INFO_LOG("開始階段結束");
}

void handle_cleanup_phase(game* gameState) {
    player* current_player = &gameState->players[gameState->now_turn_player_id];
    
    // 將出牌區的牌移到棄牌堆
    for (uint32_t i = 0; i < current_player->usecards.SIZE; i++) {
        vector_pushback(&current_player->graveyard, current_player->usecards.array[i]);
    }
    clearVector(&current_player->usecards);
    
    // 重置防禦值
    current_player->defense = 0;
}

void handle_action_phase(game* gameState) {
    // 處理玩家的行動選擇
    display_game_state(gameState);
    printf("\n可用行動:\n");
    printf("0: 專注\n");
    printf("1: 使用攻擊牌\n");
    printf("2: 使用防禦牌\n");
    printf("3: 使用移動牌\n");
    printf("4: 使用技能牌\n");
    printf("5: 使用特殊牌\n");
    printf("6: 購買卡牌\n");
    printf("7: 使用蛻變\n");
    printf("8: 使用角色特殊行動\n");
    printf("9: 丟棄毒牌\n");
    printf("10: 結束回合\n");
}

void handle_focus_action(game* gameState) {
    DEBUG_LOG("處理專注動作");
    
    player* current_player = &gameState->players[gameState->now_turn_player_id];
    
    // 專注行動: 抽一張牌並獲得1點能量
    draw_card(current_player, 1);
    current_player->energy += 1;
    
    // 更新遊戲狀態
    gameState->status = CHOOSE_MOVE;
    
    INFO_LOG("專注行動完成: 抽1張牌, +1能量");
}

void handle_attack_action(game* gameState) {
    player* current_player = &gameState->players[gameState->now_turn_player_id];
    
    printf("選擇要使用的攻擊牌 (輸入0結束):\n");
    for (uint32_t i = 0; i < current_player->hand.SIZE; i++) {
        int32_t card_id = current_player->hand.array[i];
        if (get_card_type(card_id) == CARD_TYPE_BASIC_ATK ||
            get_card_type(card_id) == CARD_TYPE_BASIC_GENERAL) {
            printf("%d: %d (攻擊力: %d)\n", i + 1, card_id, get_card_value(card_id));
        }
    }
}

void start_turn(game* gameState) {
    handle_start_phase(gameState);
    gameState->status = CHOOSE_MOVE;
}

void end_turn(game* gameState) {
    handle_cleanup_phase(gameState);
    gameState->now_turn_player_id = (gameState->now_turn_player_id + 1) % 2;
    start_turn(gameState);
}

void apply_damage(game* gameState, int target_player, int damage) {
    player* target = &gameState->players[target_player];
    
    // 先扣除防禦值
    if (target->defense > 0) {
        if (target->defense >= damage) {
            target->defense -= damage;
            damage = 0;
        } else {
            damage -= target->defense;
            target->defense = 0;
        }
    }
    
    // 扣除生命值
    if (damage > 0) {
        target->life = (target->life > damage) ? target->life - damage : 0;
    }
}

void apply_defense(game* gameState, int target_player, int defense) {
    player* target = &gameState->players[target_player];
    
    // 增加防禦值，但不超過上限
    int new_defense = target->defense + defense;
    target->defense = (new_defense <= target->maxdefense) ? new_defense : target->maxdefense;
}

bool check_attack_range(game* gameState, int attacker, int target) {
    player* attacker_player = &gameState->players[attacker];
    player* target_player = &gameState->players[target];
    
    // 計算曼哈頓距離
    int dx = abs(attacker_player->locate[0] - target_player->locate[0]);
    int dy = abs(attacker_player->locate[1] - target_player->locate[1]);
    
    // 返回是否在攻擊範圍內（射程為1）
    return (dx + dy) <= 1;
}
