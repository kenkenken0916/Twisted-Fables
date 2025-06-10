#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "card_system.h"

// 卡牌ID的編碼規則：
// TLLV
// T: 類型 (0-7)
// LL: 等級 (01-03)
// V: 變體編號 (0-9)

void init_card_system(game *gameState)
{
    srand((unsigned int)time(NULL));

    // 初始化基本牌組
    for (int type = 0; type < 3; type++)
    {
        for (int level = 0; level < 3; level++)
        {
            for (int variant = 0; variant < 18; variant++)
            {
                int32_t cardId = (type * 1000) + ((level + 1) * 100) + variant;
                create_card(cardId, type, level); // 建立卡牌屬性
                vector_pushback(&gameState->basicBuyDeck[type][level], cardId);
            }
        }
    }

    // 初始化通用牌
    for (int variant = 0; variant < 6; variant++)
    {
        int32_t cardId = (3 * 1000) + (1 * 100) + variant;
        create_card(cardId, CARD_TYPE_BASIC_GENERAL, CARD_LEVEL_1); // 建立卡牌屬性
        vector_pushback(&gameState->basicBuyDeck[3][0], cardId);
    }
}

Card create_card(int32_t id, CardType type, CardLevel level)
{
    Card card;
    card.id = id;
    card.type = type;
    card.level = level;

    // 根據卡牌類型和等級設置基礎數值
    switch (type)
    {
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

int32_t get_card_value(int32_t cardId)
{
    if (cardId <= 10)
    {
        // 基本牌值
        if (cardId >= 1 && cardId <= 3) // 攻擊牌
            return cardId * 2;
        if (cardId >= 4 && cardId <= 6) // 防禦牌
            return (cardId - 3) * 2;
        if (cardId >= 7 && cardId <= 9) // 移動牌
            return (cardId - 6) * 2;
        if (cardId == 10) // 通用牌
            return 1;
    }

    // 技能牌值（根據規格設定，11-134都是技能牌）
    if (cardId >= 11 && cardId <= 134)
    {
        return cardId % 3 + 1; // 簡單的技能值計算方式
    }

    // 蛻變牌值（135-176）
    if (cardId >= 135 && cardId <= 176)
    {
        return 3; // 蛻變牌固定值
    }

    return 0; // 未知卡牌
}

CardType get_card_type(int32_t cardId)
{
    if (cardId <= 10)
    {
        if (cardId >= 1 && cardId <= 3)
            return CARD_TYPE_BASIC_ATK;
        if (cardId >= 4 && cardId <= 6)
            return CARD_TYPE_BASIC_DEF;
        if (cardId >= 7 && cardId <= 9)
            return CARD_TYPE_BASIC_MOV;
        if (cardId == 10)
            return CARD_TYPE_BASIC_GENERAL;
    }

    // 技能牌根據角色和效果類型判斷
    if (cardId >= 11 && cardId <= 134)
    {
        // int charId = (cardId - 11) / 12;   // 每個角色12張技能牌，暫時未使用
        int skillType = (cardId - 11) % 3; // 每3張為一組（攻/防/移）

        switch (skillType)
        {
        case 0:
            return CARD_TYPE_SKILL_ATK;
        case 1:
            return CARD_TYPE_SKILL_DEF;
        case 2:
            return CARD_TYPE_SKILL_MOV;
        default:
            return CARD_TYPE_SPECIAL;
        }
    }

    // 蛻變牌
    if (cardId >= 135 && cardId <= 176)
    {
        return CARD_TYPE_SPECIAL;
    }

    return CARD_TYPE_SPECIAL;
}

CardLevel get_card_level(int32_t cardId)
{
    if (cardId <= 10)
    {
        if (cardId >= 1 && cardId <= 3)
            return cardId - 1;
        if (cardId >= 4 && cardId <= 6)
            return cardId - 4;
        if (cardId >= 7 && cardId <= 9)
            return cardId - 7;
        if (cardId == 10)
            return CARD_LEVEL_1;
    }

    // 技能牌等級（每個角色的3張同類型技能牌分別是1-3級）
    if (cardId >= 11 && cardId <= 134)
    {
        int inCharCardNum = (cardId - 11) % 12; // 角色內的第幾張牌
        int skillGroup = inCharCardNum / 3;     // 第幾組技能
        return (CardLevel)skillGroup;
    }

    // 蛻變牌
    if (cardId >= 135 && cardId <= 176)
    {
        return CARD_LEVEL_3; // 蛻變牌視為最高等級
    }

    return CARD_LEVEL_1;
}

void initial_draw(game *gameState)
{
    // 初始化玩家手牌
    for (int i = 0; i < 2; i++)
    {
        player *p = &gameState->players[i];
        int cards_to_draw = (i == 0) ? 4 : 6; // 先手4張，後手6張

        for (int j = 0; j < cards_to_draw; j++)
        {
            if (p->deck.SIZE > 0)
            {
                int32_t card = p->deck.array[p->deck.SIZE - 1];
                vector_pushback(&p->hand, card);
                vector_popback(&p->deck);
            }
        }
    }
}

bool use_card(game *gameState, int32_t cardId)
{
    player *current_player = &gameState->players[gameState->now_turn_player_id];

    // 檢查手牌中是否有這張卡
    int card_index = findVector(&current_player->hand, cardId);
    if (card_index == -1)
        return false;

    // 移除手牌，加入使用區
    eraseVector(&current_player->hand, card_index);
    vector_pushback(&current_player->usecards, cardId);

    // 根據卡牌類型執行效果
    CardType type = get_card_type(cardId);
    int32_t value = get_card_value(cardId);
    switch (type)
    {
    case CARD_TYPE_BASIC_ATK:
        gameState->nowATK += value;
        current_player->energy += value;
        break;

    case CARD_TYPE_BASIC_DEF:
        gameState->nowDEF += value;
        current_player->energy += value;
        if (current_player->defense + value <= current_player->maxdefense)
        {
            current_player->defense += value;
        }
        else
        {
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

bool buy_card(game *gameState, CardType type, CardLevel level)
{
    player *current_player = &gameState->players[gameState->now_turn_player_id];

    // 檢查玩家能量是否足夠
    if (current_player->energy < (int)(level + 1))
        return false;

    // 從對應的牌組中抽取一張牌
    vector *buy_deck = &gameState->basicBuyDeck[type][level];
    if (buy_deck->SIZE == 0)
        return false;

    // 扣除能量
    current_player->energy -= (level + 1);

    // 將卡牌加入棄牌堆
    int32_t bought_card = buy_deck->array[buy_deck->SIZE - 1];
    vector_pushback(&current_player->graveyard, bought_card);
    vector_popback(buy_deck);

    return true;
}

int32_t get_skill_cost(int32_t skillCard)
{
    // 根據技能卡ID返回能量消耗
    return (skillCard / 100) % 10; // 技能卡的百位數字表示能量消耗
}

bool needs_awakening(int32_t skillCard)
{
    // 檢查技能是否需要覺醒狀態
    return (skillCard >= 4200 && skillCard < 4300); // 睡美人的技能卡範圍
}

bool needs_ki(int32_t skillCard)
{
    // 檢查技能是否需要氣值
    return (skillCard >= 4400 && skillCard < 4500); // 花木蘭的技能卡範圍
}

int32_t required_ki(int32_t skillCard)
{
    // 返回技能需要的氣值
    if (!needs_ki(skillCard))
        return 0;
    return skillCard % 10; // 個位數表示需要的氣值
}

const char *get_card_name(int32_t cardId)
{
    switch (cardId)
    {
    // Basic cards
    case 1:
        return "LV1攻擊";
    case 2:
        return "LV2攻擊";
    case 3:
        return "LV3攻擊";
    case 4:
        return "LV1防禦";
    case 5:
        return "LV2防禦";
    case 6:
        return "LV3防禦";
    case 7:
        return "LV1移動";
    case 8:
        return "LV2移動";
    case 9:
        return "LV3移動";
    case 10:
        return "通用牌";

    // Red Hood skills
    case 11:
        return "快速射擊";
    case 12:
        return "精準射擊";
    case 13:
        return "致命狙擊";
    case 14:
        return "能量護盾";
    case 15:
        return "電流護盾";
    case 16:
        return "終極護盾";
    case 17:
        return "彈道噴射";
    case 18:
        return "火力噴射";
    case 19:
        return "暴怒噴射";

    // Snow White skills
    case 23:
        return "水晶碎片";
    case 24:
        return "水晶漩渦";
    case 25:
        return "水晶風暴";
    case 26:
        return "玷污的恩惠";
    case 27:
        return "玷污的盛筵";
    case 28:
        return "玷污的狂歡";
    case 29:
        return "破碎的幻想";
    case 30:
        return "破碎的現實";
    case 31:
        return "破碎的命運";

    // Sleeping Beauty skills
    case 35:
        return "心靈震顫";
    case 36:
        return "心靈之怒";
    case 37:
        return "心靈狂怒";
    case 38:
        return "爆裂之鎖";
    case 39:
        return "爆裂之骨";
    case 40:
        return "爆裂之魂";
    case 41:
        return "黑暗碰觸";
    case 42:
        return "黑暗糾纏";
    case 43:
        return "黑暗絞殺";

    // Alice skills
    case 47:
        return "開啟牌局";
    case 48:
        return "扭轉牌局";
    case 49:
        return "操控牌局";
    case 50:
        return "魔力技巧";
    case 51:
        return "精神幻術";
    case 52:
        return "帽子戲法";
    case 53:
        return "詭異的敏捷";
    case 54:
        return "詭異的隱蔽";
    case 55:
        return "詭異的詭異";

    // Mulan skills
    case 59:
        return "不容小覷";
    case 60:
        return "勢不可擋";
    case 61:
        return "堅不可摧";
    case 62:
        return "以靜制動";
    case 63:
        return "以柔克剛";
    case 64:
        return "以弱勝強";
    case 65:
        return "永不退縮";
    case 66:
        return "毫不留情";
    case 67:
        return "絕不饒恕";

    default:
    {
        static char buffer[32];
        snprintf(buffer, sizeof(buffer), "Card_%d", cardId);
        return buffer;
    }
    }
}
