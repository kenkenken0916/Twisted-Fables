#include <stdlib.h>
#include <time.h>
#include "utils.h"
#include "debug_log.h"

void shuffle_deck(vector* deck) {
    DEBUG_LOG("洗牌開始，牌堆大小：%u", deck->SIZE);
    
    if (deck->SIZE <= 1) return;
    
    static bool seeded = false;
    if (!seeded) {
        srand((unsigned int)time(NULL));
        seeded = true;
    }
    
    for (uint32_t i = deck->SIZE - 1; i > 0; i--) {
        uint32_t j = rand() % (i + 1);
        // 交換卡片
        int32_t temp = deck->array[i];
        deck->array[i] = deck->array[j];
        deck->array[j] = temp;
    }
    
    DEBUG_LOG("洗牌完成");
}

bool draw_card(player* p, int count) {
    DEBUG_LOG("嘗試抽取%d張牌", count);
    
    for (int i = 0; i < count; i++) {
        // 檢查牌堆是否為空
        if (p->deck.SIZE == 0) {
            // 如果棄牌堆也是空的，無法抽牌
            if (p->graveyard.SIZE == 0) {
                ERROR_LOG("無法抽牌：牌堆和棄牌堆都是空的");
                return false;
            }
            
            // 將棄牌堆洗入牌堆
            INFO_LOG("牌堆空，從棄牌堆重組");
            move_all_cards(&p->graveyard, &p->deck);
            shuffle_deck(&p->deck);
        }
        
        // 抽一張牌
        int32_t card = p->deck.array[p->deck.SIZE - 1];
        vector_pushback(&p->hand, card);
        vector_popback(&p->deck);
        
        DEBUG_LOG("抽到卡片：%d", card);
    }
    
    return true;
}

bool can_move_to(game* gameState, int player_id, int x, int y) {
    // 檢查是否超出邊界
    if (x < 0 || x > 2 || y < 0 || y > 2) {
        DEBUG_LOG("移動檢查失敗：超出邊界 (%d,%d)", x, y);
        return false;
    }
    
    // 檢查目標位置是否有其他玩家
    for (int i = 0; i < (gameState->playerMode ? 4 : 2); i++) {
        if (i != player_id && 
            gameState->players[i].locate[0] == x && 
            gameState->players[i].locate[1] == y) {
            DEBUG_LOG("移動檢查失敗：位置被占用 (%d,%d)", x, y);
            return false;
        }
    }
    
    return true;
}

int calculate_distance(int x1, int y1, int x2, int y2) {
    // 使用曼哈頓距離
    return abs(x1 - x2) + abs(y1 - y2);
}

bool is_in_range(game* gameState, int attacker_id, int target_id, int range) {
    player* attacker = &gameState->players[attacker_id];
    player* target = &gameState->players[target_id];
    
    int distance = calculate_distance(
        attacker->locate[0], attacker->locate[1],
        target->locate[0], target->locate[1]
    );
    
    DEBUG_LOG("距離檢查：攻擊者=%d，目標=%d，距離=%d，射程=%d",
             attacker_id, target_id, distance, range);
             
    return distance <= range;
}

bool has_enough_energy(player* p, int cost) {
    if (p->energy < cost) {
        DEBUG_LOG("能量不足：需要%d，現有%d", cost, p->energy);
        return false;
    }
    return true;
}

void adjust_life(player* p, int32_t amount) {
    if (!p) return;

    if (amount <= -1 && p->life > 0) {  // 使用 -1 代替 0
        uint32_t abs_amount = (uint32_t)(-amount);
        if (abs_amount > p->life) {
            p->life = 0;
        } else {
            p->life -= abs_amount;
        }
    } else if (amount > 0) {
        p->life += (uint32_t)amount;
    }
}

void adjust_defense(player* p, int32_t amount) {
    if (!p) return;

    if (amount <= -1 && p->defense > 0) {  // 使用 -1 代替 0
        uint32_t abs_amount = (uint32_t)(-amount);
        if (abs_amount > p->defense) {
            p->defense = 0;
        } else {
            p->defense -= abs_amount;
        }
    } else if (amount > 0) {
        p->defense += (uint32_t)amount;
    }
}

void move_card(vector* from, vector* to, int index) {
    if (index < 0 || (uint32_t)index >= from->SIZE) {
        ERROR_LOG("無效的卡片索引：%d", index);
        return;
    }
    
    int32_t card = from->array[index];
    vector_pushback(to, card);
    eraseVector(from, index);
    
    DEBUG_LOG("移動卡片：%d", card);
}

void move_all_cards(vector* from, vector* to) {
    DEBUG_LOG("移動所有卡片：從%p到%p", (void*)from, (void*)to);
    
    for (uint32_t i = 0; i < from->SIZE; i++) {
        vector_pushback(to, from->array[i]);
    }
    clearVector(from);
}

bool can_perform_action(game* gameState, int action_type) {
    player* current = &gameState->players[gameState->now_turn_player_id];
    
    switch (action_type) {
        case 1: // 攻擊
            return current->hand.SIZE > 0;
            
        case 2: // 防禦
            return current->hand.SIZE > 0;
            
        case 3: // 移動
            return current->hand.SIZE > 0;
            
        case 4: // 技能
            return current->energy > 0 && 
                   (current->attackSkill.SIZE > 0 || 
                    current->defenseSkill.SIZE > 0 || 
                    current->moveSkill.SIZE > 0);
            
        case 5: // 特殊卡
            return current->specialDeck.SIZE > 0;
            
        case 6: // 購買
            return current->energy > 0;
            
        default:
            return false;
    }
}
