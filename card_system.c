#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "card_system.h"

// 卡牌ID的編碼規則：
// TLLV
// T: 類型 (0-7)
// LL: 等級 (01-03)
// V: 變體編號 (0-9)

void init_card_system(game* gameState) {
    srand((unsigned int)time(NULL));
    
    // 初始化基本牌組
    for (int type = 0; type < 3; type++) {
        for (int level = 0; level < 3; level++) {
            for (int variant = 0; variant < 18; variant++) {
                int32_t cardId = (type * 1000) + ((level + 1) * 100) + variant;
                create_card(cardId, type, level); // 建立卡牌屬性
                vector_pushback(&gameState->basicBuyDeck[type][level], cardId);
            }
        }
    }
    
    // 初始化通用牌
    for (int variant = 0; variant < 6; variant++) {
        int32_t cardId = (3 * 1000) + (1 * 100) + variant;
        create_card(cardId, CARD_TYPE_BASIC_GENERAL, CARD_LEVEL_1); // 建立卡牌屬性
        vector_pushback(&gameState->basicBuyDeck[3][0], cardId);
    }
}

Card create_card(int32_t id, CardType type, CardLevel level) {
    Card card;
    card.id = id;
    card.type = type;
    card.level = level;
    
    // 根據卡牌類型和等級設置基礎數值
    switch (type) {
        case CARD_TYPE_BASIC_ATK:
            card.value = (level + 1) * 2;
            break;
        case CARD_TYPE_BASIC_DEF:
            card.value = (level + 1) * 2;
            break;
        case CARD_TYPE_BASIC_MOV:
            card.value = level + 1;
            break;
        case CARD_TYPE_BASIC_GENERAL:
            card.value = 1;
            break;
        default:
            card.value = 0;
    }
    
    // 設置卡牌消耗
    card.cost = level + 1;
    
    return card;
}

int32_t get_card_value(int32_t cardId) {
    // 根據不同卡牌類型和等級返回基礎值
    int32_t type = cardId / 1000;    // 獲取卡牌類型
    int32_t level = (cardId % 1000) / 100;  // 獲取卡牌等級
    
    switch (type) {
        case 0:  // 攻擊牌
            return level + 1;
            
        case 1:  // 防禦牌
            return level + 1;
            
        case 2:  // 移動牌
            return level + 1;
            
        case 3:  // 通用牌
            return level;
            
        default:
            return 0;
    }
}

CardType get_card_type(int32_t cardId) {
    return (CardType)((cardId / 1000) % 10);
}

CardLevel get_card_level(int32_t cardId) {
    return (CardLevel)(((cardId / 100) % 10) - 1);
}

void initial_draw(game* gameState) {
    // 初始化玩家手牌
    for (int i = 0; i < 2; i++) {
        player* p = &gameState->players[i];
        int cards_to_draw = (i == 0) ? 4 : 6; // 先手4張，後手6張
        
        for (int j = 0; j < cards_to_draw; j++) {
            if (p->deck.SIZE > 0) {
                int32_t card = p->deck.array[p->deck.SIZE - 1];
                vector_pushback(&p->hand, card);
                vector_popback(&p->deck);
            }
        }
    }
}

bool use_card(game* gameState, int32_t cardId) {
    player* current_player = &gameState->players[gameState->now_turn_player_id];
    
    // 檢查手牌中是否有這張卡
    int card_index = findVector(&current_player->hand, cardId);
    if (card_index == -1) return false;
    
    // 移除手牌，加入使用區
    eraseVector(&current_player->hand, card_index);
    vector_pushback(&current_player->usecards, cardId);
    
    // 根據卡牌類型執行效果
    CardType type = get_card_type(cardId);
    int32_t value = get_card_value(cardId);
      switch (type) {
        case CARD_TYPE_BASIC_ATK:
            gameState->nowATK += value;
            current_player->energy += value;
            break;
            
        case CARD_TYPE_BASIC_DEF:
            gameState->nowDEF += value;
            current_player->energy += value;
            if (current_player->defense + value <= current_player->maxdefense) {
                current_player->defense += value;
            } else {
                current_player->defense = current_player->maxdefense;
            }
            break;
            
        case CARD_TYPE_BASIC_MOV:
            gameState->nowMOV += value;
            current_player->energy += value;
            break;
            
        case CARD_TYPE_BASIC_GENERAL:
            // 通用牌可以當作任何基本牌使用
            break;
            
        case CARD_TYPE_SKILL_ATK:
        case CARD_TYPE_SKILL_DEF:
        case CARD_TYPE_SKILL_MOV:
            // 技能卡需要配合基本卡使用
            // 交給 card_combination 處理
            break;
            
        case CARD_TYPE_SPECIAL:
            // 特殊卡需要由角色系統處理
            // 交給 character_system 處理
            break;
    }
    
    return true;
}

bool buy_card(game* gameState, CardType type, CardLevel level) {
    player* current_player = &gameState->players[gameState->now_turn_player_id];
    
    // 檢查玩家能量是否足夠
    if (current_player->energy < (int)(level + 1)) return false;
    
    // 從對應的牌組中抽取一張牌
    vector* buy_deck = &gameState->basicBuyDeck[type][level];
    if (buy_deck->SIZE == 0) return false;
    
    // 扣除能量
    current_player->energy -= (level + 1);
    
    // 將卡牌加入棄牌堆
    int32_t bought_card = buy_deck->array[buy_deck->SIZE - 1];
    vector_pushback(&current_player->graveyard, bought_card);
    vector_popback(buy_deck);
    
    return true;
}

int32_t get_skill_cost(int32_t skillCard) {
    // 根據技能卡ID返回能量消耗
    return (skillCard / 100) % 10;  // 技能卡的百位數字表示能量消耗
}

bool needs_awakening(int32_t skillCard) {
    // 檢查技能是否需要覺醒狀態
    return (skillCard >= 4200 && skillCard < 4300);  // 睡美人的技能卡範圍
}

bool needs_ki(int32_t skillCard) {
    // 檢查技能是否需要氣值
    return (skillCard >= 4400 && skillCard < 4500);  // 花木蘭的技能卡範圍
}

int32_t required_ki(int32_t skillCard) {
    // 返回技能需要的氣值
    if (!needs_ki(skillCard)) return 0;
    return skillCard % 10;  // 個位數表示需要的氣值
}
