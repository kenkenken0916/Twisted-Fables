#include "game_logic.h"
#include "game_state.h"
#include "../systems/card_system.h"
#include "../systems/character_system.h"
#include "../utils/debug_log.h"
#include "../utils/utils.h"

bool process_turn(game *gameState)
{
    if (!gameState)
    {
        ERROR_LOG("Invalid game state pointer");
        return false;
    }

    player *currentPlayer = &gameState->players[gameState->now_turn_player_id];
    GAME_LOG(LOG_DEBUG, "Processing turn for player %d (character %d)",
             gameState->now_turn_player_id, currentPlayer->character);

    // Handle different game states
    switch (gameState->status)
    {
    case CHOOSE_MOVE:
        return handle_action_phase(gameState);

    case USE_ATK:
    case USE_DEF:
    case USE_MOV:
        return handle_basic_card_usage(gameState);

    case USE_SKILL:
        return handle_skill_usage(gameState);

    case BUY_CARD_TYPE:
        return handle_card_purchase(gameState);

    default:
        return handle_special_state(gameState);
    }
}

bool is_game_ended(game *gameState)
{
    if (!gameState)
    {
        return true; // Error state, end game
    }

    // Check if any player has 0 or less life
    for (int i = 0; i < (gameState->playerMode == 0 ? 2 : 4); i++)
    {
        if (gameState->players[i].life <= 0)
        {
            GAME_LOG(LOG_INFO, "Player %d defeated with %d life", i, gameState->players[i].life);
            return true;
        }
    }

    return false;
}

bool handle_action_phase(game *gameState)
{
    if (!gameState)
    {
        ERROR_LOG("Invalid game state pointer");
        return false;
    }

    player *currentPlayer = &gameState->players[gameState->now_turn_player_id];
    GAME_LOG(LOG_DEBUG, "Action phase for player %d", gameState->now_turn_player_id);

    // Show available actions
    show_available_actions(gameState);

    // Process action based on game state
    switch (gameState->status)
    {
    case CHOOSE_MOVE:
        // Player can:
        // 1. Use basic cards (attack, defense, movement)
        // 2. Use skill cards
        // 3. Buy cards
        // 4. Focus (remove a card)
        // 5. End turn
        break;

    case FOCUS:
        // Handle focus action
        break;

    default:
        WARNING_LOG("Unexpected state in action phase: %d", gameState->status);
        break;
    }

    return true;
}

void show_available_actions(game *gameState)
{
    if (!gameState)
        return;

    player *currentPlayer = &gameState->players[gameState->now_turn_player_id];

    GAME_LOG(LOG_DEBUG, "Available actions for player %d:", gameState->now_turn_player_id);
    GAME_LOG(LOG_DEBUG, "Energy: %d", currentPlayer->energy);
    GAME_LOG(LOG_DEBUG, "Life: %d/%d", currentPlayer->life, currentPlayer->maxlife);
    GAME_LOG(LOG_DEBUG, "Defense: %d/%d", currentPlayer->defense, currentPlayer->maxdefense);

    // Show hand
    GAME_LOG(LOG_DEBUG, "Hand (%d cards):", currentPlayer->hand.SIZE);
    for (uint32_t i = 0; i < currentPlayer->hand.SIZE; i++)
    {
        int32_t cardId = currentPlayer->hand.array[i];
        GAME_LOG(LOG_DEBUG, "- Card %d (Type: %d)", cardId, get_card_type(cardId));
    }
}

bool handle_basic_card_usage(game *gameState)
{
    player *currentPlayer = &gameState->players[gameState->now_turn_player_id];
    (void)currentPlayer; // Suppress unused variable warning

    switch (gameState->status)
    {
    case USE_ATK:
        return process_attack_cards(gameState);
    case USE_DEF:
        return process_defense_cards(gameState);
    case USE_MOV:
        return process_movement_cards(gameState);
    default:
        ERROR_LOG("Invalid basic card usage state");
        return false;
    }
}

bool handle_skill_usage(game *gameState)
{
    player *currentPlayer = &gameState->players[gameState->now_turn_player_id];
    (void)currentPlayer; // Suppress unused variable warning

    GAME_LOG(LOG_DEBUG, "Processing skill usage for player %d", gameState->now_turn_player_id);

    // Validate skill usage requirements
    if (!validate_skill_usage(gameState))
    {
        ERROR_LOG("Invalid skill usage attempt");
        return false;
    }

    // Execute skill effect
    return execute_skill_effect(gameState);
}

bool handle_card_purchase(game *gameState)
{
    player *currentPlayer = &gameState->players[gameState->now_turn_player_id];
    (void)currentPlayer; // Suppress unused variable warning

    GAME_LOG(LOG_DEBUG, "Processing card purchase for player %d", gameState->now_turn_player_id);

    // Show available cards for purchase
    show_purchasable_cards(gameState);

    return true;
}

bool handle_special_state(game *gameState)
{
    GAME_LOG(LOG_DEBUG, "Processing special state: %d", gameState->status);

    // Handle character-specific states
    switch (gameState->status)
    {
    case CHOOSE_IDENTITY:
        return handle_alice_identity_choice(gameState);

    case CHOOSE_TENTACLE_LOCATION:
        return handle_mermaid_tentacle_placement(gameState);

    case USE_AWAKEN_TOKEN:
        return handle_sleeping_beauty_awaken(gameState);

    case GET_KI:
        return handle_mulan_ki_generation(gameState);

    case CHOOSE_TOKEN:
        return handle_scheherazade_token_choice(gameState);

    default:
        WARNING_LOG("Unhandled special state: %d", gameState->status);
        return true;
    }
}

void switch_to_next_player(game *gameState)
{
    if (!gameState)
    {
        ERROR_LOG("Invalid game state pointer");
        return;
    }

    int playerCount = (gameState->playerMode == 0) ? 2 : 4;

    // In 2v2 mode, check for defeated players
    if (gameState->playerMode == 1)
    {
        // Handle special 2v2 turn order when a player is defeated
        handle_2v2_turn_order(gameState);
    }
    else
    {
        // Simple 1v1 turn switching
        gameState->now_turn_player_id = (gameState->now_turn_player_id + 1) % playerCount;
    }

    GAME_LOG(LOG_DEBUG, "Switched to player %d", gameState->now_turn_player_id);
}

int8_t get_game_winner(game *gameState)
{
    if (!gameState)
    {
        return -1;
    }

    if (gameState->playerMode == 0)
    {
        // 1v1 mode
        if (gameState->players[0].life <= 0)
            return 1;
        if (gameState->players[1].life <= 0)
            return 0;
    }
    else
    {
        // 2v2 mode - team wins if opposing team is eliminated
        bool team0_alive = (gameState->players[0].life > 0) || (gameState->players[2].life > 0);
        bool team1_alive = (gameState->players[1].life > 0) || (gameState->players[3].life > 0);

        if (!team0_alive)
            return 1; // Team 1 wins
        if (!team1_alive)
            return 0; // Team 0 wins
    }

    return -1; // No winner yet
}

bool advance_turn_phase(game *gameState)
{
    if (!gameState)
    {
        ERROR_LOG("Invalid game state pointer");
        return false;
    }

    player *currentPlayer = &gameState->players[gameState->now_turn_player_id];
    (void)currentPlayer; // Suppress unused variable warning

    // Execute turn phases in order
    static bool start_phase_done = false;
    static bool cleanup_phase_done = false;
    static bool action_phase_done = false;

    if (!start_phase_done)
    {
        if (!handle_start_phase(gameState))
            return false;
        start_phase_done = true;
        return true;
    }

    if (!cleanup_phase_done)
    {
        if (!handle_cleanup_phase(gameState))
            return false;
        cleanup_phase_done = true;
        return true;
    }

    if (!action_phase_done)
    {
        if (!handle_action_phase(gameState))
            return false;
        action_phase_done = true;
        return true;
    }

    // End phase
    if (!handle_end_phase(gameState))
        return false;

    // Reset phase flags for next turn
    start_phase_done = false;
    cleanup_phase_done = false;
    action_phase_done = false;

    // Switch to next player
    switch_to_next_player(gameState);

    return true;
}

bool handle_start_phase(game *gameState)
{
    player *currentPlayer = &gameState->players[gameState->now_turn_player_id];

    GAME_LOG(LOG_DEBUG, "Start phase for player %d", gameState->now_turn_player_id);

    // Resolve ongoing effects
    resolve_ongoing_effects(gameState);

    // Handle character-specific start phase effects
    handle_character_start_phase(gameState, currentPlayer);

    return true;
}

bool handle_cleanup_phase(game *gameState)
{
    if (!gameState)
    {
        ERROR_LOG("Invalid game state pointer");
        return false;
    }

    player *currentPlayer = &gameState->players[gameState->now_turn_player_id];
    GAME_LOG(LOG_DEBUG, "Cleanup phase for player %d", gameState->now_turn_player_id);

    // Discard cards in play area (except ongoing effects)
    discard_used_cards(currentPlayer);

    // Reset defense value to 0
    currentPlayer->defense = 0;

    return true;
}

bool handle_end_phase(game *gameState)
{
    if (!gameState)
    {
        ERROR_LOG("Invalid game state pointer");
        return false;
    }

    player *currentPlayer = &gameState->players[gameState->now_turn_player_id];
    GAME_LOG(LOG_DEBUG, "End phase for player %d", gameState->now_turn_player_id);

    // Reset energy to 0
    currentPlayer->energy = 0;

    // Discard hand
    discard_hand(currentPlayer);

    // Discard cards in play (except ongoing effects)
    discard_used_cards(currentPlayer);

    // Draw 6 cards
    draw_cards(currentPlayer, 6);

    // Resolve end-of-turn effects
    resolve_end_turn_effects(gameState);

    return true;
}

// Helper functions (to be implemented)
void discard_hand(player *currentPlayer)
{
    if (!currentPlayer)
        return;

    while (currentPlayer->hand.SIZE > 0)
    {
        int32_t cardId = currentPlayer->hand.array[currentPlayer->hand.SIZE - 1];
        popbackVector(&currentPlayer->hand);
        pushbackVector(&currentPlayer->graveyard, cardId);
    }
}

void discard_used_cards(player *currentPlayer)
{
    if (!currentPlayer)
        return;

    while (currentPlayer->usecards.SIZE > 0)
    {
        int32_t cardId = currentPlayer->usecards.array[currentPlayer->usecards.SIZE - 1];
        popbackVector(&currentPlayer->usecards);
        // Check if card has ongoing effects before discarding
        pushbackVector(&currentPlayer->graveyard, cardId);
    }
}

void draw_cards(player *currentPlayer, int count)
{
    if (!currentPlayer || count <= 0)
        return;

    for (int i = 0; i < count && currentPlayer->deck.SIZE > 0; i++)
    {
        int32_t cardId = currentPlayer->deck.array[currentPlayer->deck.SIZE - 1];
        popbackVector(&currentPlayer->deck);
        pushbackVector(&currentPlayer->hand, cardId);
    }

    // If deck is empty and need more cards, shuffle graveyard into deck
    if ((uint32_t)count > currentPlayer->hand.SIZE && currentPlayer->graveyard.SIZE > 0)
    {
        shuffle_graveyard_to_deck(currentPlayer);
        draw_cards(currentPlayer, count - currentPlayer->hand.SIZE);
    }
}

void shuffle_graveyard_to_deck(player *currentPlayer)
{
    // Move all cards from graveyard to deck and shuffle
    while (currentPlayer->graveyard.SIZE > 0)
    {
        int32_t cardId = currentPlayer->graveyard.array[0];
        eraseVector(&currentPlayer->graveyard, 0);
        pushbackVector(&currentPlayer->deck, cardId);
    }

    // Shuffle deck (implementation in utils.c)
    shuffle_vector(&currentPlayer->deck);
}

void resolve_ongoing_effects(game *gameState)
{
    (void)gameState; // Suppress unused parameter warning
    // Implementation for ongoing effect resolution
    GAME_LOG(LOG_DEBUG, "Resolving ongoing effects");
}

void handle_2v2_turn_order(game *gameState)
{
    (void)gameState; // Suppress unused parameter warning
    // Implementation for 2v2 special turn handling
    GAME_LOG(LOG_DEBUG, "Handling 2v2 turn order");
}

void resolve_end_turn_effects(game *gameState)
{
    (void)gameState; // Suppress unused parameter warning
    // Implementation for end-of-turn effect resolution
    GAME_LOG(LOG_DEBUG, "Resolving end-of-turn effects");
}

// Character-specific handlers (placeholders)
bool handle_alice_identity_choice(game *gameState)
{
    if (!gameState)
    {
        ERROR_LOG("Invalid game state pointer");
        return false;
    }

    player *currentPlayer = &gameState->players[gameState->now_turn_player_id];

    // Only Alice can choose identity
    if (currentPlayer->character != 3)
    {
        ERROR_LOG("Identity choice for non-Alice character");
        return false;
    }

    // First turn allows choosing any identity
    if (gameState->turnCount == 1)
    {
        // Wait for player choice
        if (currentPlayer->alice.identity == 0)
        {
            GAME_LOG(LOG_DEBUG, "Waiting for initial identity choice");
            return true;
        }
    }

    // Must switch to different identity each turn
    uint8_t currentIdentity = currentPlayer->alice.identity;
    for (uint8_t newIdentity = 1; newIdentity <= 3; newIdentity++)
    {
        if (newIdentity != currentIdentity)
        {
            if (alice_switch_identity(currentPlayer, newIdentity))
            {
                GAME_LOG(LOG_DEBUG, "Switched to identity %d", newIdentity);
                return true;
            }
        }
    }

    ERROR_LOG("Failed to switch Alice's identity");
    return false;
}

bool handle_mermaid_tentacle_placement(game *gameState)
{
    (void)gameState; // Suppress unused parameter warning
    GAME_LOG(LOG_DEBUG, "Handling Mermaid tentacle placement");
    return true;
}

bool handle_sleeping_beauty_awaken(game *gameState)
{
    (void)gameState; // Suppress unused parameter warning
    GAME_LOG(LOG_DEBUG, "Handling Sleeping Beauty awaken token usage");
    return true;
}

bool handle_mulan_ki_generation(game *gameState)
{
    (void)gameState; // Suppress unused parameter warning
    GAME_LOG(LOG_DEBUG, "Handling Mulan Ki generation");
    return true;
}

bool handle_scheherazade_token_choice(game *gameState)
{
    (void)gameState; // Suppress unused parameter warning
    GAME_LOG(LOG_DEBUG, "Handling Scheherazade token choice");
    return true;
}
