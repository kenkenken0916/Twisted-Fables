#ifndef _CARD_COMBINATION_H
#define _CARD_COMBINATION_H

#include "architecture.h"
#include "card_system.h"

// 卡牌組合結構
typedef struct {
    int32_t mainCard;      // 主要卡牌
    int32_t supportCard;   // 支援卡牌
    bool isActive;         // 是否處於活動狀態
    int32_t duration;      // 持續回合數
} CardCombination;

// 檢查卡牌組合是否有效
bool check_card_combination(game* gameState, int32_t card1, int32_t card2);

// 處理卡牌組合效果
void apply_card_combination(game* gameState, int32_t card1, int32_t card2);

// 檢查技能卡是否需要基本卡
bool skill_needs_basic_card(int32_t skillCard);

// 獲取技能卡所需的基本卡類型
CardType get_required_basic_card_type(int32_t skillCard);

// 檢查持續效果
void check_ongoing_effects(game* gameState);

// 移除已過期的持續效果
void remove_expired_effects(game* gameState);

#endif // _CARD_COMBINATION_H
