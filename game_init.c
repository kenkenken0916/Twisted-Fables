#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "game_init.h"
#include "debug_log.h"
#include "character_system.h"

void init_game(game *gameState)
{
    INFO_LOG("初始化遊戲");

    // 初始化遊戲模式為1v1
    gameState->playerMode = 0;
    gameState->now_turn_player_id = 0;
    gameState->status = CHOOSE_IDENTITY;

    // 初始化玩家
    for (int i = 0; i < 2; i++)
    {
        init_player(&gameState->players[i]);

        // Set default starting positions
        gameState->players[i].locate[0] = i == 0 ? 3 : 5; // Player 1 at position 3, Player 2 at position 5
        gameState->players[i].locate[1] = 0;
    }

    // 初始化牌組
    init_decks(gameState);

    // 初始化遊戲狀態相關變數
    gameState->nowATK = 0;
    gameState->nowDEF = 0;
    gameState->nowMOV = 0;
    gameState->nowUsingCardID = 0;
    gameState->totalDamage = 0;

    // 初始化各種牌堆
    vector_init(&gameState->nowShowingCards);
    vector_init(&gameState->relicDeck);
    vector_init(&gameState->relicGraveyard);
    vector_init(&gameState->tentacle_TOKEN_locate);

    DEBUG_LOG("遊戲初始化完成");
}

void init_player(player *p)
{
    DEBUG_LOG("初始化玩家");

    // 初始化玩家的基本屬性
    p->team = 0;
    p->locate[0] = 0;
    p->locate[1] = 0;
    p->character = 0;
    p->maxlife = 0;
    p->life = 0;
    p->maxdefense = 0;
    p->defense = 0;
    p->energy = 0;
    p->specialGate = 0;

    // 初始化玩家的牌組
    vector_init(&p->hand);
    vector_init(&p->deck);
    vector_init(&p->usecards);
    vector_init(&p->graveyard);
    vector_init(&p->metamorphosis);
    vector_init(&p->attackSkill);
    vector_init(&p->defenseSkill);
    vector_init(&p->moveSkill);
    vector_init(&p->specialDeck);
    // 初始化角色特殊屬性
    memset(&p->redHood, 0, sizeof(p->redHood));
    vector_init(&p->snowWhite.remindPosion);
    memset(&p->sleepingBeauty, 0, sizeof(p->sleepingBeauty));
    memset(&p->alice, 0, sizeof(p->alice));
    memset(&p->mulan, 0, sizeof(p->mulan));
    memset(&p->kaguya, 0, sizeof(p->kaguya));
    memset(&p->mermaid, 0, sizeof(p->mermaid));
    memset(&p->matchGirl, 0, sizeof(p->matchGirl));
    memset(&p->dorothy, 0, sizeof(p->dorothy));
    vector_init(&p->scheherazade.destiny_TOKEN_locate);
    vector_init(&p->scheherazade.destiny_TOKEN_type);

    INFO_LOG("玩家初始化完成");
}

void init_decks(game *gameState)
{
    // 初始化基本牌組
    init_basic_cards(gameState);

    // 初始化特殊牌組
    init_special_cards(gameState);
}

void init_basic_cards(game *gameState)
{
    // 初始化攻擊、防禦、移動和通用牌組
    for (int type = 0; type < 4; type++)
    {
        for (int level = 0; level < 3; level++)
        {
            vector_init(&gameState->basicBuyDeck[type][level]);

            // 根據不同類型填充牌組
            if (type == 3)
            { // 通用牌
                if (level == 0)
                { // 只有LV1通用牌
                    for (int i = 0; i < 6; i++)
                    {
                        vector_pushback(&gameState->basicBuyDeck[type][level], 10);
                    }
                }
            }
            else
            {
                int cardId;
                switch (type)
                {
                case 0:                 // 攻擊牌
                    cardId = level + 1; // LV1=1, LV2=2, LV3=3
                    break;
                case 1:                 // 防禦牌
                    cardId = level + 4; // LV1=4, LV2=5, LV3=6
                    break;
                case 2:                 // 移動牌
                    cardId = level + 7; // LV1=7, LV2=8, LV3=9
                    break;
                }
                for (int i = 0; i < 18; i++)
                {
                    vector_pushback(&gameState->basicBuyDeck[type][level], cardId);
                }
            }
        }
    }
}

void init_special_cards(game *gameState)
{
    if (!gameState)
        return;
    // 在選擇角色後被調用
    // 根據玩家的角色選擇來初始化特殊牌組
    for (int i = 0; i < 2; i++)
    {
        player *current = &gameState->players[i];
        if (current->character != 0)
        { // 如果角色已選擇
            // TODO: 根據角色ID初始化特殊牌組
            (void)current; // 暫時避免警告，等待實現
        }
    }
}
