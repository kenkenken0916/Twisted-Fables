#ifndef _CHARACTER_SYSTEM_H
#define _CHARACTER_SYSTEM_H

#include "architecture.h"
#include "card_system.h"

// 角色ID定義
typedef enum {
    CHAR_RED_HOOD = 0,    // 小紅帽
    CHAR_SNOW_WHITE = 1,  // 白雪公主
    CHAR_SLEEPING = 2,    // 睡美人
    CHAR_ALICE = 3,       // 愛麗絲
    CHAR_MULAN = 4,       // 花木蘭
    CHAR_KAGUYA = 5,      // 輝夜姬
    CHAR_MERMAID = 6,     // 美人魚
    CHAR_MATCH_GIRL = 7,  // 賣火柴女孩
    CHAR_DOROTHY = 8,     // 桃樂絲
    CHAR_SCHEHERAZADE = 9 // 山魯佐德
} CharacterID;

// 角色基礎屬性
typedef struct {
    int maxLife;
    int maxDefense;
    int specialGate;
    const char* name;
    const char* description;
} CharacterBase;

// 初始化角色
void init_character(game* gameState, int playerIndex, CharacterID charId);

// 設置角色初始牌組
void setup_initial_deck(player* p);

// 處理角色特殊技能
bool handle_character_skill(game* gameState, int32_t skillCard);

// 處理角色被動效果
void handle_character_passive(game* gameState);

// 檢查技能是否可用
bool can_use_skill(game* gameState, int32_t skillCard);

// 獲取角色資訊
const CharacterBase* get_character_info(CharacterID charId);

// 實現各個角色的特殊技能函數
bool red_hood_skill(game* gameState, int32_t skillCard);
bool snow_white_skill(game* gameState, int32_t skillCard);
bool sleeping_beauty_skill(game* gameState, int32_t skillCard);
bool alice_skill(game* gameState, int32_t skillCard);
bool mulan_skill(game* gameState, int32_t skillCard);
bool kaguya_skill(game* gameState, int32_t skillCard);
bool mermaid_skill(game* gameState, int32_t skillCard);
bool match_girl_skill(game* gameState, int32_t skillCard);
bool dorothy_skill(game* gameState, int32_t skillCard);
bool scheherazade_skill(game* gameState, int32_t skillCard);

// 卡牌相關函數聲明
int32_t get_card_value(int32_t cardId);
int32_t get_skill_cost(int32_t skillCard);
bool needs_awakening(int32_t skillCard);
bool needs_ki(int32_t skillCard);
int32_t required_ki(int32_t skillCard);

#endif // _CHARACTER_SYSTEM_H
