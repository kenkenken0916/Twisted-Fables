#include "character_system.h"
#include "debug_log.h"

// Character base attributes definition
static const CharacterBase characterData[] = {
    {30, 6, 15, "Little Red Riding Hood", "A ranged attacker specializing in archery"},
    {34, 6, 17, "Snow White", "A controller using poison tactics"},
    {42, 6, 21, "Sleeping Beauty", "Can switch between sleep and awakened states"},
    {32, 6, 16, "Alice", "A versatile character who can change identities"},
    {34, 3, 17, "Mulan", "Uses Ki energy to enhance abilities"},
    {32, 6, 16, "Princess Kaguya", "Excels in defense and counter-attacks"},
    {36, 3, 18, "The Little Mermaid", "Controls the field with tentacles"},
    {36, 6, 18, "The Little Match Girl", "Uses matches for burn attacks"},
    {40, 6, 20, "Dorothy", "A combo master who can chain attacks"},
    {36, 6, 18, "Scheherazade", "A tactician using destiny tokens"}};

void init_character(game *gameState, int playerIndex, CharacterID charId)
{
    DEBUG_LOG("Initializing character: Player %d selected %s", playerIndex + 1, characterData[charId].name);

    player *p = &gameState->players[playerIndex];
    p->character = charId;
    p->maxlife = characterData[charId].maxLife;
    p->life = p->maxlife;
    p->maxdefense = characterData[charId].maxDefense;
    p->defense = 0;
    p->energy = 0;
    p->specialGate = characterData[charId].specialGate;

    // Log character initialization
    INFO_LOG("Player %d HP set to %d/%d", playerIndex + 1, p->life, p->maxlife);

    // 初始化角色特殊屬性
    switch (charId)
    {
    case CHAR_RED_HOOD:
        memset(p->redHood.saveCard, 0, sizeof(p->redHood.saveCard));
        break;

    case CHAR_SNOW_WHITE:
        vector_init(&p->snowWhite.remindPosion);
        break;

    case CHAR_SLEEPING:
        p->sleepingBeauty.AWAKEN_TOKEN = 0;
        p->sleepingBeauty.AWAKEN = 0;
        p->sleepingBeauty.dayNightmareDrawRemind = 0;
        p->sleepingBeauty.atkRise = 0;
        p->sleepingBeauty.atkRiseTime = 0;
        p->sleepingBeauty.usedmeta1 = 0;
        break;

    case CHAR_ALICE:
        p->alice.identity = 0;
        p->alice.riseBasic = 0;
        p->alice.restartTurn = 0;
        p->alice.havedrestart = 0;
        break;

    case CHAR_MULAN:
        p->mulan.KI_TOKEN = 0;
        p->mulan.extraCard = 0;
        p->mulan.extraDraw = 0;
        break;

    case CHAR_KAGUYA:
        p->kaguya.useDefenseAsATK = 0;
        p->kaguya.useMoveTarget = 0;
        break;

    case CHAR_MERMAID:
        // 美人魚當前不需要特殊初始化
        break;

    case CHAR_MATCH_GIRL:
        p->matchGirl.remindMatch = 0;
        p->matchGirl.pushedMatch = 0;
        break;

    case CHAR_DOROTHY:
        p->dorothy.COMBO_TOKEN = 0;
        p->dorothy.canCombo = 0;
        break;

    case CHAR_SCHEHERAZADE:
        vector_init(&p->scheherazade.destiny_TOKEN_locate);
        vector_init(&p->scheherazade.destiny_TOKEN_type);
        p->scheherazade.selectToken = 0;
        break;

    default:
        ERROR_LOG("未知的角色ID: %d", charId);
        break;
    }

    setup_initial_deck(p);
}

void setup_initial_deck(player *p)
{
    if (!p)
        return;

    DEBUG_LOG("設置玩家%d的初始牌組", p->character + 1);

    // 添加基本牌
    for (int i = 0; i < 3; i++)
    {
        // 等級1攻擊牌
        vector_pushback(&p->deck, 1);
        // 等級1防禦牌
        vector_pushback(&p->deck, 4);
        // 等級1移動牌
        vector_pushback(&p->deck, 7);
    }

    // 添加角色特有的技能牌
    switch (p->character)
    {
    case CHAR_RED_HOOD:
        // 添加快速射擊
        vector_pushback(&p->attackSkill, 11);
        // 添加能量護盾
        vector_pushback(&p->defenseSkill, 14);
        break;

    case CHAR_SNOW_WHITE:
        vector_pushback(&p->attackSkill, 23);
        vector_pushback(&p->defenseSkill, 26);
        vector_pushback(&p->moveSkill, 29);
        break;

    case CHAR_SLEEPING:
        vector_pushback(&p->attackSkill, 35);
        vector_pushback(&p->defenseSkill, 38);
        vector_pushback(&p->moveSkill, 41);
        break;

    case CHAR_ALICE:
        vector_pushback(&p->attackSkill, 47);
        vector_pushback(&p->defenseSkill, 50);
        vector_pushback(&p->moveSkill, 53);
        break;

    case CHAR_MULAN:
        vector_pushback(&p->attackSkill, 59);
        vector_pushback(&p->defenseSkill, 62);
        vector_pushback(&p->moveSkill, 65);
        break;

    case CHAR_KAGUYA:
        vector_pushback(&p->attackSkill, 71);
        vector_pushback(&p->defenseSkill, 74);
        vector_pushback(&p->moveSkill, 77);
        break;

    case CHAR_MERMAID:
        vector_pushback(&p->attackSkill, 83);
        vector_pushback(&p->defenseSkill, 86);
        vector_pushback(&p->moveSkill, 89);
        break;

    case CHAR_MATCH_GIRL:
        vector_pushback(&p->attackSkill, 95);
        vector_pushback(&p->defenseSkill, 98);
        vector_pushback(&p->moveSkill, 101);
        break;

    case CHAR_DOROTHY:
        vector_pushback(&p->attackSkill, 107);
        vector_pushback(&p->defenseSkill, 110);
        vector_pushback(&p->moveSkill, 113);
        break;

    case CHAR_SCHEHERAZADE:
        vector_pushback(&p->attackSkill, 119);
        vector_pushback(&p->defenseSkill, 122);
        vector_pushback(&p->moveSkill, 125);
        break;

    default:
        ERROR_LOG("未知的角色ID: %d", p->character);
        break;
    }

    // 洗混牌堆
    shuffle_deck(&p->deck);
}

bool handle_character_skill(game *gameState, int32_t skillCard)
{
    if (!gameState)
        return false;
    player *current = &gameState->players[gameState->now_turn_player_id];
    if (!current)
        return false;

    switch (current->character)
    {
    case CHAR_RED_HOOD:
        return red_hood_skill(gameState, skillCard);

    case CHAR_SNOW_WHITE:
        return snow_white_skill(gameState, skillCard);

    case CHAR_SLEEPING:
        return sleeping_beauty_skill(gameState, skillCard);

    case CHAR_ALICE:
        return alice_skill(gameState, skillCard);

    case CHAR_MULAN:
        return mulan_skill(gameState, skillCard);

    case CHAR_KAGUYA:
        return kaguya_skill(gameState, skillCard);

    case CHAR_MERMAID:
        return mermaid_skill(gameState, skillCard);

    case CHAR_MATCH_GIRL:
        return match_girl_skill(gameState, skillCard);

    case CHAR_DOROTHY:
        return dorothy_skill(gameState, skillCard);

    case CHAR_SCHEHERAZADE:
        return scheherazade_skill(gameState, skillCard);

    default:
        return false;
    }
    return false;
}

// 小紅帽的技能實現
bool red_hood_skill(game *gameState, int32_t skillCard)
{
    DEBUG_LOG("執行小紅帽技能：%d", skillCard);

    // 快速射擊
    if (skillCard == 4001)
    {
        // 射程1，傷害1+基本牌值
        gameState->nowATK = 1 + get_card_value(gameState->nowUsingCardID);
        return true;
    }

    // 精準射擊
    if (skillCard == 4002)
    {
        // 射程2，傷害2+基本牌值
        gameState->nowATK = 2 + get_card_value(gameState->nowUsingCardID);
        return true;
    }

    // ... 其他技能

    return false;
}

// 白雪公主的技能實現
bool snow_white_skill(game *gameState, int32_t skillCard)
{
    (void)gameState;
    (void)skillCard;
    // TODO: 實現白雪公主的技能邏輯
    return false;
}

// 睡美人的技能實現
bool sleeping_beauty_skill(game *gameState, int32_t skillCard)
{
    (void)gameState;
    (void)skillCard;
    // TODO: 實現睡美人的技能邏輯
    return false;
}

bool alice_skill(game *gameState, int32_t skillCard)
{
    (void)gameState;
    (void)skillCard;
    // TODO: 實現愛麗絲的技能邏輯
    return false;
}

bool mulan_skill(game *gameState, int32_t skillCard)
{
    (void)gameState;
    (void)skillCard;
    // TODO: 實現花木蘭的技能邏輯
    return false;
}

bool kaguya_skill(game *gameState, int32_t skillCard)
{
    (void)gameState;
    (void)skillCard;
    // TODO: 實現輝夜姬的技能邏輯
    return false;
}

bool mermaid_skill(game *gameState, int32_t skillCard)
{
    (void)gameState;
    (void)skillCard;
    // TODO: 實現美人魚的技能邏輯
    return false;
}

bool match_girl_skill(game *gameState, int32_t skillCard)
{
    (void)gameState;
    (void)skillCard;
    // TODO: 實現賣火柴女孩的技能邏輯
    return false;
}

bool dorothy_skill(game *gameState, int32_t skillCard)
{
    (void)gameState;
    (void)skillCard;
    // TODO: 實現桃樂絲的技能邏輯
    return false;
}

bool scheherazade_skill(game *gameState, int32_t skillCard)
{
    (void)gameState;
    (void)skillCard;
    // TODO: 實現山魯佐德的技能邏輯
    return false;
}

bool can_use_skill(game *gameState, int32_t skillCard)
{
    player *current = &gameState->players[gameState->now_turn_player_id];

    // 檢查能量是否足夠
    int skillCost = get_skill_cost(skillCard);
    if (current->energy < skillCost)
    {
        ERROR_LOG("能量不足：需要%d，現有%d", skillCost, current->energy);
        return false;
    }

    // 檢查技能特殊條件
    switch (current->character)
    {
    case CHAR_SLEEPING:
        // 檢查睡美人的覺醒狀態
        if (needs_awakening(skillCard) && !current->sleepingBeauty.AWAKEN)
        {
            ERROR_LOG("需要處於覺醒狀態");
            return false;
        }
        break;

    case CHAR_MULAN:
        // 檢查氣值
        if (needs_ki(skillCard) && current->mulan.KI_TOKEN < (uint32_t)required_ki(skillCard))
        {
            ERROR_LOG("氣值不足");
            return false;
        }
        break;

        // ... 其他角色的特殊條件
    }

    return true;
}

const CharacterBase *get_character_info(CharacterID charId)
{
    if (charId < 0 || charId >= sizeof(characterData) / sizeof(CharacterBase))
    {
        ERROR_LOG("無效的角色ID：%d", charId);
        return NULL;
    }
    return &characterData[charId];
}
