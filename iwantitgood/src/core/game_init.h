#ifndef _GAME_INIT_H
#define _GAME_INIT_H

#include "../utils/architecture.h"

/**
 * Setup initial game configuration
 * @param gameState Pointer to game state
 * @return true if successful, false otherwise
 */
bool setup_initial_game(game *gameState);

/**
 * Initialize a player with default values
 * @param player Pointer to player structure
 * @param playerIndex Player index (0-3)
 * @return true if successful, false otherwise
 */
bool init_player(player *player, int playerIndex);

/**
 * Cleanup player resources
 * @param player Pointer to player structure
 */
void cleanup_player(player *player);

/**
 * Validate player state
 * @param player Pointer to player structure
 * @return true if valid, false otherwise
 */
bool validate_player(player *player);

/**
 * Setup initial deck for a character
 * @param player Pointer to player structure
 * @param characterId Character ID
 * @return true if successful, false otherwise
 */
bool setup_initial_deck(player *player, uint8_t characterId);

/**
 * Setup basic card supply decks
 * @param gameState Pointer to game state
 * @return true if successful, false otherwise
 */
bool setup_basic_supply_decks(game *gameState);

/**
 * Setup character skill supply decks
 * @param gameState Pointer to game state
 * @return true if successful, false otherwise
 */
bool setup_skill_supply_decks(game *gameState);

/**
 * Setup initial hand for players
 * @param gameState Pointer to game state
 * @return true if successful, false otherwise
 */
bool setup_initial_hands(game *gameState);

/**
 * Setup initial positioning on battle tracks
 * @param gameState Pointer to game state
 * @return true if successful, false otherwise
 */
bool setup_initial_positions(game *gameState);

/**
 * Initialize character-specific data
 * @param player Pointer to player structure
 */
void init_character_specific_data(player *player);

/**
 * Set character base stats
 * @param player Pointer to player structure
 * @param characterId Character ID
 */
void set_character_stats(player *player, uint8_t characterId);

/**
 * Add initial character skills to deck
 * @param player Pointer to player structure
 * @param characterId Character ID
 */
void add_initial_character_skills(player *player, uint8_t characterId);

#endif // _GAME_INIT_H
