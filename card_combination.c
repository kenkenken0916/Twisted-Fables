#include "card_combination.h"
#include "debug_log.h"

bool check_card_combination(game* gameState, int32_t card1, int32_t card2) {
    (void)gameState;  // 標記參數為有意未使用
    DEBUG_LOG("檢查卡牌組合：%d 和 %d", card1, card2);
    
    CardType type1 = get_card_type(card1);
    CardType type2 = get_card_type(card2);
    
    // 如果是技能卡，檢查是否需要基本卡
    if (type1 >= CARD_TYPE_SKILL_ATK && type1 <= CARD_TYPE_SKILL_MOV) {
        if (!skill_needs_basic_card(card1)) {
            DEBUG_LOG("技能卡 %d 不需要基本卡", card1);
            return true;
        }
        
        CardType required = get_required_basic_card_type(card1);
        if (type2 != required && type2 != CARD_TYPE_BASIC_GENERAL) {
            ERROR_LOG("技能卡 %d 需要類型 %d 的基本卡，但收到類型 %d", card1, required, type2);
            return false;
        }
    }
    
    DEBUG_LOG("卡牌組合有效");
    return true;
}

void apply_card_combination(game* gameState, int32_t card1, int32_t card2) {
    DEBUG_LOG("應用卡牌組合效果：%d 和 %d", card1, card2);
    
    CardType type1 = get_card_type(card1);
    int32_t value1 = get_card_value(card1);
    int32_t value2 = get_card_value(card2);
    
    // 根據不同的卡牌類型處理效果
    switch (type1) {
        case CARD_TYPE_SKILL_ATK:
            gameState->nowATK = value1 + value2;
            INFO_LOG("技能攻擊組合：基礎攻擊 %d + 技能加成 %d", value2, value1);
            break;
            
        case CARD_TYPE_SKILL_DEF:
            gameState->nowDEF = value1 + value2;
            INFO_LOG("技能防禦組合：基礎防禦 %d + 技能加成 %d", value2, value1);
            break;
            
        case CARD_TYPE_SKILL_MOV:
            gameState->nowMOV = value1 + value2;
            INFO_LOG("技能移動組合：基礎移動 %d + 技能加成 %d", value2, value1);
            break;
            
        default:
            WARN_LOG("未知的技能卡類型：%d", type1);
            break;
    }
}

bool skill_needs_basic_card(int32_t skillCard) {
    // 從卡牌ID解析是否需要基本卡
    CardType type = get_card_type(skillCard);
    if (type < CARD_TYPE_SKILL_ATK || type > CARD_TYPE_SKILL_MOV) {
        return false;
    }
    
    // 在這裡可以加入特殊技能的判斷
    // 例如某些特殊技能不需要基本卡
    
    return true;
}

CardType get_required_basic_card_type(int32_t skillCard) {
    CardType type = get_card_type(skillCard);
    switch (type) {
        case CARD_TYPE_SKILL_ATK:
            return CARD_TYPE_BASIC_ATK;
        case CARD_TYPE_SKILL_DEF:
            return CARD_TYPE_BASIC_DEF;
        case CARD_TYPE_SKILL_MOV:
            return CARD_TYPE_BASIC_MOV;
        default:
            return CARD_TYPE_BASIC_GENERAL;
    }
}
