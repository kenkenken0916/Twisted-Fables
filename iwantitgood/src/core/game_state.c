#include "game_state.h"
#include "game_init.h"
#include "../utils/debug_log.h"
#include "../utils/utils.h"

bool init_game_state(game *gameState)
{
    if (!gameState)
    {
        ERROR_LOG("Invalid game state pointer");
        return false;
    }

    GAME_LOG(LOG_DEBUG, "Initializing game state");

    // Initialize basic game properties
    gameState->now_turn_player_id = 0;
    gameState->playerMode = 0; // Default to 1v1 mode
    gameState->relicMode = 0;
    gameState->status = CHOOSE_MOVE;

    // Initialize metadata
    gameState->nowATK = 0;
    gameState->nowDEF = 0;
    gameState->nowMOV = 0;
    gameState->nowUsingCardID = -1;
    gameState->totalDamage = 0;
    gameState->nowShowingCards = initVector();

    // Initialize tentacle locations (for Little Mermaid)
    gameState->tentacle_TOKEN_locate = initVector();

    // Initialize relic system
    for (int i = 0; i < 11; i++)
    {
        gameState->relic[i] = 0;
    }
    gameState->relicDeck = initVector();
    gameState->relicGraveyard = initVector();

    // Initialize basic card supply decks
    for (int type = 0; type < 4; type++)
    {
        for (int level = 0; level < 3; level++)
        {
            gameState->basicBuyDeck[type][level] = initVector();
        }
    }

    // Initialize players
    for (int i = 0; i < 4; i++)
    {
        if (!init_player(&gameState->players[i], i))
        {
            ERROR_LOG("Failed to initialize player %d", i);
            return false;
        }
    }

    // Setup initial game based on mode
    if (!setup_initial_game(gameState))
    {
        ERROR_LOG("Failed to setup initial game");
        return false;
    }

    GAME_LOG(LOG_INFO, "Game state initialized successfully");
    return true;
}

bool update_game_state(game *gameState)
{
    if (!gameState)
    {
        ERROR_LOG("Invalid game state pointer");
        return false;
    }

    // Validate state consistency
    if (!validate_game_state(gameState))
    {
        WARNING_LOG("Game state validation failed");
        return false;
    }

    // Update turn-based effects
    update_turn_effects(gameState);

    // Update character-specific states
    update_character_states(gameState);

    return true;
}

void cleanup_game_state(game *gameState)
{
    if (!gameState)
    {
        WARNING_LOG("Invalid game state pointer during cleanup");
        return;
    }

    GAME_LOG(LOG_DEBUG, "Cleaning up game state");

    // Clear all vectors
    clearVector(&gameState->nowShowingCards);
    clearVector(&gameState->tentacle_TOKEN_locate);
    clearVector(&gameState->relicDeck);
    clearVector(&gameState->relicGraveyard);

    // Clear basic card supply decks
    for (int type = 0; type < 4; type++)
    {
        for (int level = 0; level < 3; level++)
        {
            clearVector(&gameState->basicBuyDeck[type][level]);
        }
    }

    // Cleanup players
    for (int i = 0; i < 4; i++)
    {
        cleanup_player(&gameState->players[i]);
    }

    GAME_LOG(LOG_DEBUG, "Game state cleanup completed");
}

bool validate_game_state(game *gameState)
{
    if (!gameState)
    {
        ERROR_LOG("Game state is NULL");
        return false;
    }

    // Validate basic properties
    if (gameState->now_turn_player_id < 0 ||
        gameState->now_turn_player_id >= (gameState->playerMode == 0 ? 2 : 4))
    {
        ERROR_LOG("Invalid current player ID: %d", gameState->now_turn_player_id);
        return false;
    }

    if (gameState->playerMode < 0 || gameState->playerMode > 1)
    {
        ERROR_LOG("Invalid player mode: %d", gameState->playerMode);
        return false;
    }

    // Validate energy limits
    if (gameState->nowATK < 0 || gameState->nowDEF < 0 || gameState->nowMOV < 0)
    {
        ERROR_LOG("Invalid energy values: ATK=%d, DEF=%d, MOV=%d",
                  gameState->nowATK, gameState->nowDEF, gameState->nowMOV);
        return false;
    }

    // Validate players
    for (int i = 0; i < (gameState->playerMode == 0 ? 2 : 4); i++)
    {
        if (!validate_player(&gameState->players[i]))
        {
            ERROR_LOG("Player %d validation failed", i);
            return false;
        }
    }

    return true;
}

void reset_game_state(game *gameState)
{
    if (!gameState)
    {
        ERROR_LOG("Invalid game state pointer");
        return;
    }

    GAME_LOG(LOG_DEBUG, "Resetting game state");

    // Cleanup current state
    cleanup_game_state(gameState);

    // Reinitialize
    init_game_state(gameState);
}

bool save_game_state(game *gameState)
{
    if (!gameState)
    {
        ERROR_LOG("Invalid game state pointer");
        return false;
    }

    // For now, just log that we would save
    // In a full implementation, this would serialize the game state
    GAME_LOG(LOG_DEBUG, "Game state saved (placeholder)");
    return true;
}

bool restore_game_state(game *gameState)
{
    if (!gameState)
    {
        ERROR_LOG("Invalid game state pointer");
        return false;
    }

    // For now, just log that we would restore
    // In a full implementation, this would deserialize the game state
    GAME_LOG(LOG_DEBUG, "Game state restored (placeholder)");
    return true;
}

// Helper functions
void update_turn_effects(game *gameState)
{
    // Update ongoing effects that are time-based
    player *currentPlayer = &gameState->players[gameState->now_turn_player_id];
    (void)currentPlayer; // Suppress unused variable warning

    // Example: Reduce duration of temporary effects
    // This would be implemented based on specific effect systems

    GAME_LOG(LOG_DEBUG, "Updated turn effects for player %d", gameState->now_turn_player_id);
}

void update_character_states(game *gameState)
{
    // Update character-specific states
    for (int i = 0; i < (gameState->playerMode == 0 ? 2 : 4); i++)
    {
        player *p = &gameState->players[i];

        switch (p->character)
        {
        case 2: // Sleeping Beauty
            update_sleeping_beauty_state(p);
            break;
        case 4: // Mulan
            update_mulan_state(p);
            break;
        case 6: // Little Mermaid
            update_mermaid_state(p, gameState);
            break;
        case 8: // Dorothy
            update_dorothy_state(p);
            break;
        case 9: // Scheherazade
            update_scheherazade_state(p);
            break;
        default:
            // No special state updates needed
            break;
        }
    }
}

void update_sleeping_beauty_state(player *p)
{
    // Update awakening state and token effects
    if (p->sleepingBeauty.AWAKEN_TOKEN > 6)
    {
        p->sleepingBeauty.AWAKEN_TOKEN = 6; // Max 6 tokens
    }
}

void update_mulan_state(player *p)
{
    // Update Ki token state
    if (p->mulan.KI_TOKEN > 12)
    {
        p->mulan.KI_TOKEN = 12; // Max 12 Ki tokens
    }
}

void update_mermaid_state(player *p, game *gameState)
{
    (void)p;         // Suppress unused parameter warning
    (void)gameState; // Suppress unused parameter warning
    // Update tentacle positions and effects
    // Tentacle positions are stored in gameState->tentacle_TOKEN_locate
}

void update_dorothy_state(player *p)
{
    // Update combo token state
    if (p->dorothy.COMBO_TOKEN > 12)
    {
        p->dorothy.COMBO_TOKEN = 12; // Max 12 combo tokens
    }
}

void update_scheherazade_state(player *p)
{
    // Update destiny token state
    if (p->scheherazade.destiny_TOKEN_locate.SIZE > 6)
    {
        // Max 6 destiny tokens, remove excess
        while (p->scheherazade.destiny_TOKEN_locate.SIZE > 6)
        {
            popbackVector(&p->scheherazade.destiny_TOKEN_locate);
            popbackVector(&p->scheherazade.destiny_TOKEN_type);
        }
    }
}
