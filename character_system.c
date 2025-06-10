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

    // 添加基本牌 (3張1階攻擊、3張1階防禦、3張1階移動，共9張)
    for (int i = 0; i < 3; i++)
    {
        // 等級1攻擊牌
        vector_pushback(&p->deck, 1); // LV1 Attack card
        // 等級1防禦牌
        vector_pushback(&p->deck, 4); // LV1 Defense card
        // 等級1移動牌
        vector_pushback(&p->deck, 7); // LV1 Move card
    }

    // 添加1階技能牌 (每種技能1張，共3張)
    switch (p->character)
    {
    case CHAR_RED_HOOD:
        // 每種1階技能牌各一張
        vector_pushback(&p->deck, 11); // 快速射擊 (攻擊)
        vector_pushback(&p->deck, 14); // 能量護盾 (防禦)
        vector_pushback(&p->deck, 17); // 彈道噴射 (移動)
        // 存儲所有技能牌供後續使用
        vector_pushback(&p->attackSkill, 11);
        vector_pushback(&p->attackSkill, 12);
        vector_pushback(&p->attackSkill, 13);
        vector_pushback(&p->defenseSkill, 14);
        vector_pushback(&p->defenseSkill, 15);
        vector_pushback(&p->defenseSkill, 16);
        vector_pushback(&p->moveSkill, 17);
        vector_pushback(&p->moveSkill, 18);
        vector_pushback(&p->moveSkill, 19);
        break;

    case CHAR_SNOW_WHITE:
        vector_pushback(&p->deck, 23); // 水晶碎片 (1階攻擊)
        vector_pushback(&p->deck, 26); // 玷污的恩惠 (1階防禦)
        vector_pushback(&p->deck, 29); // 破碎的幻想 (1階移動)
        // 存儲所有技能牌
        vector_pushback(&p->attackSkill, 23);
        vector_pushback(&p->attackSkill, 24);
        vector_pushback(&p->attackSkill, 25);
        vector_pushback(&p->defenseSkill, 26);
        vector_pushback(&p->defenseSkill, 27);
        vector_pushback(&p->defenseSkill, 28);
        vector_pushback(&p->moveSkill, 29);
        vector_pushback(&p->moveSkill, 30);
        vector_pushback(&p->moveSkill, 31);
        break;

    case CHAR_SLEEPING:
        vector_pushback(&p->deck, 35); // 心靈震顫 (1階攻擊)
        vector_pushback(&p->deck, 38); // 爆裂之鎖 (1階防禦)
        vector_pushback(&p->deck, 41); // 黑暗碰觸 (1階移動)
        // 存儲所有技能牌
        vector_pushback(&p->attackSkill, 35);
        vector_pushback(&p->attackSkill, 36);
        vector_pushback(&p->attackSkill, 37);
        vector_pushback(&p->defenseSkill, 38);
        vector_pushback(&p->defenseSkill, 39);
        vector_pushback(&p->defenseSkill, 40);
        vector_pushback(&p->moveSkill, 41);
        vector_pushback(&p->moveSkill, 42);
        vector_pushback(&p->moveSkill, 43);
        break;

    case CHAR_ALICE:
        vector_pushback(&p->deck, 47); // 開啟牌局 (1階攻擊)
        vector_pushback(&p->deck, 50); // 魔力技巧 (1階防禦)
        vector_pushback(&p->deck, 53); // 詭異的敏捷 (1階移動)
        // 存儲所有技能牌
        vector_pushback(&p->attackSkill, 47);
        vector_pushback(&p->attackSkill, 48);
        vector_pushback(&p->attackSkill, 49);
        vector_pushback(&p->defenseSkill, 50);
        vector_pushback(&p->defenseSkill, 51);
        vector_pushback(&p->defenseSkill, 52);
        vector_pushback(&p->moveSkill, 53);
        vector_pushback(&p->moveSkill, 54);
        vector_pushback(&p->moveSkill, 55);
        break;

    case CHAR_MULAN:
        vector_pushback(&p->deck, 59); // 不容小覷 (1階攻擊)
        vector_pushback(&p->deck, 62); // 以靜制動 (1階防禦)
        vector_pushback(&p->deck, 65); // 永不退縮 (1階移動)
        // 存儲所有技能牌
        vector_pushback(&p->attackSkill, 59);
        vector_pushback(&p->attackSkill, 60);
        vector_pushback(&p->attackSkill, 61);
        vector_pushback(&p->defenseSkill, 62);
        vector_pushback(&p->defenseSkill, 63);
        vector_pushback(&p->defenseSkill, 64);
        vector_pushback(&p->moveSkill, 65);
        vector_pushback(&p->moveSkill, 66);
        vector_pushback(&p->moveSkill, 67);
        break;

    case CHAR_KAGUYA:
        vector_pushback(&p->deck, 71); // 1階攻擊技能
        vector_pushback(&p->deck, 74); // 1階防禦技能
        vector_pushback(&p->deck, 77); // 1階移動技能
        // 存儲所有技能牌
        vector_pushback(&p->attackSkill, 71);
        vector_pushback(&p->attackSkill, 72);
        vector_pushback(&p->attackSkill, 73);
        vector_pushback(&p->defenseSkill, 74);
        vector_pushback(&p->defenseSkill, 75);
        vector_pushback(&p->defenseSkill, 76);
        vector_pushback(&p->moveSkill, 77);
        vector_pushback(&p->moveSkill, 78);
        vector_pushback(&p->moveSkill, 79);
        break;

    case CHAR_MERMAID:
        vector_pushback(&p->deck, 83); // 1階攻擊技能
        vector_pushback(&p->deck, 86); // 1階防禦技能
        vector_pushback(&p->deck, 89); // 1階移動技能
        // 存儲所有技能牌
        vector_pushback(&p->attackSkill, 83);
        vector_pushback(&p->attackSkill, 84);
        vector_pushback(&p->attackSkill, 85);
        vector_pushback(&p->defenseSkill, 86);
        vector_pushback(&p->defenseSkill, 87);
        vector_pushback(&p->defenseSkill, 88);
        vector_pushback(&p->moveSkill, 89);
        vector_pushback(&p->moveSkill, 90);
        vector_pushback(&p->moveSkill, 91);
        break;

    case CHAR_MATCH_GIRL:
        vector_pushback(&p->deck, 95);  // 1階攻擊技能
        vector_pushback(&p->deck, 98);  // 1階防禦技能
        vector_pushback(&p->deck, 101); // 1階移動技能
        // 存儲所有技能牌
        vector_pushback(&p->attackSkill, 95);
        vector_pushback(&p->attackSkill, 96);
        vector_pushback(&p->attackSkill, 97);
        vector_pushback(&p->defenseSkill, 98);
        vector_pushback(&p->defenseSkill, 99);
        vector_pushback(&p->defenseSkill, 100);
        vector_pushback(&p->moveSkill, 101);
        vector_pushback(&p->moveSkill, 102);
        vector_pushback(&p->moveSkill, 103);
        break;

    case CHAR_DOROTHY:
        vector_pushback(&p->deck, 107); // 1階攻擊技能
        vector_pushback(&p->deck, 110); // 1階防禦技能
        vector_pushback(&p->deck, 113); // 1階移動技能
        // 存儲所有技能牌
        vector_pushback(&p->attackSkill, 107);
        vector_pushback(&p->attackSkill, 108);
        vector_pushback(&p->attackSkill, 109);
        vector_pushback(&p->defenseSkill, 110);
        vector_pushback(&p->defenseSkill, 111);
        vector_pushback(&p->defenseSkill, 112);
        vector_pushback(&p->moveSkill, 113);
        vector_pushback(&p->moveSkill, 114);
        vector_pushback(&p->moveSkill, 115);
        break;

    case CHAR_SCHEHERAZADE:
        vector_pushback(&p->deck, 119); // 1階攻擊技能
        vector_pushback(&p->deck, 122); // 1階防禦技能
        vector_pushback(&p->deck, 125); // 1階移動技能
        // 存儲所有技能牌
        vector_pushback(&p->attackSkill, 119);
        vector_pushback(&p->attackSkill, 120);
        vector_pushback(&p->attackSkill, 121);
        vector_pushback(&p->defenseSkill, 122);
        vector_pushback(&p->defenseSkill, 123);
        vector_pushback(&p->defenseSkill, 124);
        vector_pushback(&p->moveSkill, 125);
        vector_pushback(&p->moveSkill, 126);
        vector_pushback(&p->moveSkill, 127);
        break;

    default:
        ERROR_LOG("未知的角色ID: %d", p->character);
        break;
    }

    // 洗混牌堆
    shuffle_deck(&p->deck);

    DEBUG_LOG("牌堆設置完成，共%d張牌", p->deck.SIZE);
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
