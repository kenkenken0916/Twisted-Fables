#ifndef _GAME_STATE_H
#define _GAME_STATE_H

#include "../utils/architecture.h"

/**
 * Initialize game state
 * @param gameState Pointer to game state
 * @return true if successful, false otherwise
 */
bool init_game_state(game *gameState);

/**
 * Update game state
 * @param gameState Pointer to game state
 * @return true if successful, false otherwise
 */
bool update_game_state(game *gameState);

/**
 * Cleanup game state
 * @param gameState Pointer to game state
 */
void cleanup_game_state(game *gameState);

/**
 * Validate game state consistency
 * @param gameState Pointer to game state
 * @return true if valid, false otherwise
 */
bool validate_game_state(game *gameState);

/**
 * Reset game state to initial values
 * @param gameState Pointer to game state
 */
void reset_game_state(game *gameState);

/**
 * Save game state (for recovery)
 * @param gameState Pointer to game state
 * @return true if successful, false otherwise
 */
bool save_game_state(game *gameState);

/**
 * Restore game state from saved state
 * @param gameState Pointer to game state
 * @return true if successful, false otherwise
 */
bool restore_game_state(game *gameState);

/**
 * Update turn-specific effects
 * @param gameState Pointer to game state
 */
void update_turn_effects(game *gameState);

/**
 * Update character-specific states
 * @param gameState Pointer to game state
 */
void update_character_states(game *gameState);

/**
 * Update Sleeping Beauty state
 * @param p Pointer to player
 */
void update_sleeping_beauty_state(player *p);

/**
 * Update Mulan state
 * @param p Pointer to player
 */
void update_mulan_state(player *p);

/**
 * Update Mermaid state
 * @param p Pointer to player
 * @param gameState Pointer to game state
 */
void update_mermaid_state(player *p, game *gameState);

/**
 * Update Dorothy state
 * @param p Pointer to player
 */
void update_dorothy_state(player *p);

/**
 * Update Scheherazade state
 * @param p Pointer to player
 */
void update_scheherazade_state(player *p);

#endif // _GAME_STATE_H
