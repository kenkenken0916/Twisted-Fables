#ifndef _CARD_SYSTEM_H
#define _CARD_SYSTEM_H

#include "architecture.h"
#include "game_state.h"

// Get card name based on card ID
const char *get_card_name(int32_t cardId);

// 卡牌類型定義
typedef enum
{
    CARD_TYPE_BASIC_ATK = 0,
    CARD_TYPE_BASIC_DEF = 1,
    CARD_TYPE_BASIC_MOV = 2,
    CARD_TYPE_BASIC_GENERAL = 3,
    CARD_TYPE_SKILL_ATK = 4,
    CARD_TYPE_SKILL_DEF = 5,
    CARD_TYPE_SKILL_MOV = 6,
    CARD_TYPE_SPECIAL = 7
} CardType;

// 卡牌等級定義
typedef enum
{
    CARD_LEVEL_1 = 0,
    CARD_LEVEL_2 = 1,
    CARD_LEVEL_3 = 2
} CardLevel;

// 卡牌結構
typedef struct
{
    int32_t id;      // 卡牌唯一ID
    CardType type;   // 卡牌類型
    CardLevel level; // 卡牌等級
    int32_t value;   // 卡牌數值（攻擊/防禦/移動值）
    int32_t cost;    // 卡牌消耗
} Card;

// 卡牌初始化函數
void init_card_system(game *gameState);

// 卡牌創建函數
Card create_card(int32_t id, CardType type, CardLevel level);

// 獲取卡牌數值
int32_t get_card_value(int32_t cardId);

// 獲取卡牌類型
CardType get_card_type(int32_t cardId);

// 獲取卡牌等級
CardLevel get_card_level(int32_t cardId);

// 遊戲開始時抽牌
void initial_draw(game *gameState);

// 使用卡牌
bool use_card(game *gameState, int32_t cardId);

// 購買卡牌
bool buy_card(game *gameState, CardType type, CardLevel level);

// 獲取技能卡消耗
int32_t get_skill_cost(int32_t skillCard);

// 技能卡相關判斷
bool needs_awakening(int32_t skillCard);
bool needs_ki(int32_t skillCard);
int32_t required_ki(int32_t skillCard);

#endif // _CARD_SYSTEM_H
