#include "mainloop.h"
#include "game_logic.h"
#include "game_init.h"
#include "game_state.h"
#include "../systems/card_system.h"
#include "../systems/character_system.h"
#include "../utils/debug_log.h"
#include "../utils/utils.h"
#include"../systems/state_system.h"
#include "../utils/vector.h"

void print_game_state(const game *gameState)
{
    if (!gameState)
    {
        printf("Game state is NULL!\n");
        return;
    }

    printf("\n=================== Game State ===================\n");
    printf("Game Mode: %s (%d)\n", gameState->playerMode == 0 ? "1v1" : "2v2", gameState->playerMode);
    printf("Current Turn Player: %d\n", gameState->now_turn_player_id);
    printf("Game Status: %d\n", gameState->status);
    printf("Relic Mode: %d\n", gameState->relicMode);

    // Print current action values
    printf("\nCurrent Action Values:\n");
    printf("ATK: %d, DEF: %d, MOV: %d\n", gameState->nowATK, gameState->nowDEF, gameState->nowMOV);
    printf("Total Damage: %d\n", gameState->totalDamage);
    printf("Current Card ID: %d\n", gameState->nowUsingCardID);

    // Print player information
    for (int i = 0; i < (gameState->playerMode == 0 ? 2 : 4); i++)
    {
        const player *p = &gameState->players[i];
        printf("\n----------------- Player %d -----------------\n", i);
        printf("Character: %d\n", p->character);
        printf("Team: %d\n", p->team);
        printf("Position: [%d, %d]\n", p->locate[0], p->locate[1]);
        printf("Life: %d/%d\n", p->life, p->maxlife);
        printf("Defense: %d/%d\n", p->defense, p->maxdefense);
        printf("Energy: %d\n", p->energy);
        printf("Special Gate: %d\n", p->specialGate);

        // Deck information
        printf("\nDeck Status:\n");
        printf("Hand: %d cards\n", p->hand.SIZE);
        printf("Deck: %d cards\n", p->deck.SIZE);
        printf("Graveyard: %d cards\n", p->graveyard.SIZE);
        printf("Used Cards: %d\n", p->usecards.SIZE);

        // Special decks
        printf("\nSpecial Decks:\n");
        printf("Metamorphosis: %d cards\n", p->metamorphosis.SIZE);
        printf("Attack Skills: %d cards\n", p->attackSkill.SIZE);
        printf("Defense Skills: %d cards\n", p->defenseSkill.SIZE);
        printf("Move Skills: %d cards\n", p->moveSkill.SIZE);
        printf("Special Deck: %d cards\n", p->specialDeck.SIZE);

        // Character-specific information
        printf("\nCharacter Specific Info:\n");
        switch (p->character)
        {
        case 0: // Red Riding Hood
            printf("Red Hood Save Cards: [%d, %d, %d]\n",
                   p->redHood.saveCard[0], p->redHood.saveCard[1], p->redHood.saveCard[2]);
            break;

        case 1: // Snow White
            printf("Poison Reminders: %d\n", p->snowWhite.remindPosion.SIZE);
            break;

        case 2: // Sleeping Beauty
            printf("Awaken Token: %d\n", p->sleepingBeauty.AWAKEN_TOKEN);
            printf("Awaken State: %d\n", p->sleepingBeauty.AWAKEN);
            printf("ATK Rise: %d (Times: %d)\n", p->sleepingBeauty.atkRise, p->sleepingBeauty.atkRiseTime);
            printf("Meta Used: [%d, %d]\n", p->sleepingBeauty.usedmeta1, p->sleepingBeauty.usedmeta2);
            break;

        case 3: // Alice
            printf("Identity: %d\n", p->alice.identity);
            printf("Rise Basic: %d\n", p->alice.riseBasic);
            printf("Restart Turn: %d (Used: %d)\n", p->alice.restartTurn, p->alice.havedrestart);
            break;

        case 4: // Mulan
            printf("KI Token: %d\n", p->mulan.KI_TOKEN);
            printf("Extra Card: %d\n", p->mulan.extraCard);
            printf("Extra Draw: %d\n", p->mulan.extraDraw);
            break;

        case 5: // Kaguya
            printf("Use Defense as ATK: %d\n", p->kaguya.useDefenseAsATK);
            printf("Use Move Target: %d\n", p->kaguya.useMoveTarget);
            break;

        case 7: // Match Girl
            printf("Remind Match: %d\n", p->matchGirl.remindMatch);
            printf("Pushed Match: %d\n", p->matchGirl.pushedMatch);
            break;

        case 8: // Dorothy
            printf("Combo Token: %d\n", p->dorothy.COMBO_TOKEN);
            printf("Can Combo: %d\n", p->dorothy.canCombo);
            break;

        case 9: // Scheherazade
            printf("Destiny Token Locations: %d\n", p->scheherazade.destiny_TOKEN_locate.SIZE);
            printf("Destiny Token Types: %d\n", p->scheherazade.destiny_TOKEN_type.SIZE);
            printf("Selected Token: %d\n", p->scheherazade.selectToken);
            break;
        }
    }

    // Print tentacle locations for mermaid
    if (gameState->tentacle_TOKEN_locate.SIZE > 0)
    {
        printf("\nTentacle Locations:\n");
        for (uint32_t i = 0; i < gameState->tentacle_TOKEN_locate.SIZE; i++)
        {
            printf("Tentacle %d: Position %d\n", i, gameState->tentacle_TOKEN_locate.array[i]);
        }
    }

    // Print currently showing cards
    if (gameState->nowShowingCards.SIZE > 0)
    {
        printf("\nCurrently Showing Cards:\n");
        for (uint32_t i = 0; i < gameState->nowShowingCards.SIZE; i++)
        {
            printf("Card %d: %d\n", i + 1, gameState->nowShowingCards.array[i]);
        }
    }

    printf("\n================ End Game State ================\n\n");
}

int mainloop(int argc, char *argv[])
{
    (void)argc; // Suppress unused parameter warning
    (void)argv; // Suppress unused parameter warning

    game gameState;

    DEBUG_LOG("Initializing game systems");

    // Initialize game systems
    if (!init_game_systems(&gameState))
    {
        ERROR_LOG("Failed to initialize game systems");
        return EXIT_FAILURE;
    }

    // Parse command line arguments
    if (argc > 1)
    {
        // Handle command line options (future feature)
        DEBUG_LOG("Command line arguments provided: %d", argc);
    }

    // Initialize game state
    if (!init_game_state(&gameState))
    {
        ERROR_LOG("Failed to initialize game state");
        cleanup_game_systems(&gameState);
        return EXIT_FAILURE;
    }

    DEBUG_LOG("Starting main game loop");

    // Main game loop
    print_game_state(&gameState); // Print initial game state
    printf("player1 charactor:");
    scanf("%d", &gameState.players[0].character);
    printf("player2 charactor:");
    scanf("%d", &gameState.players[1].character);

    while (process_game_frame(&gameState))
    {
        // Game continues until process_game_frame returns false
        // All game logic is handled within process_game_frame
        int32_t nowplayer = gameState.now_turn_player_id;
        int32_t now_enemy=nowplayer==0?1:0;
        int32_t choose_move;
        handle_choose_move(&gameState,&choose_move);
        DEBUG_LOG("Processing game frame for player %d", nowplayer);
        switch (choose_move)
        {
        case 0:
            int32_t focus_card,card_id;

            handle_remove_hg(&gameState,&focus_card);
            DEBUG_LOG("Player %d chose to focus on card %d", nowplayer, focus_card);
            //getVector(&gameState.players[nowplayer].hand, focus_card,&card_id);
            eraseVector(&gameState.players[nowplayer].hand, focus_card-1);
            DEBUG_LOG("Card %d removed from player %d's hand", focus_card, nowplayer);
            gameState.now_turn_player_id=(nowplayer+1)%2;// Switch to next player
            break;
        case 1:
            int32_t choose_idx[10]={};
            int32_t amount;
            amount=handle_use_atk(&gameState,choose_idx);
            gameState.nowATK=0;
            for(int i=0;i<amount;i++){
                pushbackVector(&gameState.players[nowplayer].usecards, choose_idx[i]);
                DEBUG_LOG("Player %d used attack card %d", nowplayer, choose_idx[i]);
                gameState.nowATK+=choose_idx[i];
                gameState.players[nowplayer].energy+=choose_idx[i];
                deleteValueVector(&gameState.players[nowplayer].hand, choose_idx[i]);
            }            
            break;
        case 2:
            /* code */
            break;
        case 3:
            break;
        case 4:
            /* code */
            break;
        case 5:
            break;
        case 6:
            /* code */
            break;
        case 7:
            break;
        case 8:
            /* code */
            break;
        case 9:
            break;
        case 10:
            break;
        default:
            break;
        }
        
    }

    DEBUG_LOG("Game loop ended");

    // Cleanup
    cleanup_game_systems(&gameState);

    return EXIT_SUCCESS;
}

bool init_game_systems(game *gameState)
{
    if (!gameState)
    {
        ERROR_LOG("Invalid game state pointer");
        return false;
    }

    DEBUG_LOG("Initializing card system");
    if (!init_card_system())
    {
        ERROR_LOG("Failed to initialize card system");
        return false;
    }

    DEBUG_LOG("Initializing character system");
    if (!init_character_system())
    {
        ERROR_LOG("Failed to initialize character system");
        return false;
    }

    DEBUG_LOG("All game systems initialized successfully");
    return true;
}

void cleanup_game_systems(game *gameState)
{
    if (!gameState)
    {
        WARNING_LOG("Invalid game state pointer during cleanup");
        return;
    }

    DEBUG_LOG("Cleaning up game systems");

    // Cleanup character system
    cleanup_character_system();

    // Cleanup card system
    cleanup_card_system();

    // Cleanup game state
    cleanup_game_state(gameState);

    DEBUG_LOG("Game systems cleanup completed");
}

bool process_game_frame(game *gameState)
{
    if (!gameState)
    {
        ERROR_LOG("Invalid game state pointer");
        return false;
    }

    // Check for game end conditions
    if (is_game_ended(gameState))
    {
        DEBUG_LOG("Game has ended");
        return false;
    }

    // // Process current turn
    // if (!process_turn(gameState))
    // {
    //     ERROR_LOG("Error processing turn");
    //     return false;
    // }

    // // Update game state
    // if (!update_game_state(gameState))
    // {
    //     ERROR_LOG("Error updating game state");
    //     return false;
    // }

    return true;
}
