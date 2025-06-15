#ifndef _CHARACTER_SYSTEM_H
#define _CHARACTER_SYSTEM_H

#include "../utils/architecture.h"

/**
 * Initialize the character system
 * @return true if successful, false otherwise
 */
bool init_character_system(void);

/**
 * Cleanup the character system
 */
void cleanup_character_system(void);

/**
 * Get character name by ID
 * @param characterId Character ID (0-9)
 * @return Character name string
 */
const char *get_character_name(uint8_t characterId);

/**
 * Get character's base stats
 * @param characterId Character ID
 * @param life Pointer to store max life
 * @param defense Pointer to store max defense
 * @param ultimateThreshold Pointer to store ultimate threshold
 * @return true if valid character, false otherwise
 */
bool get_character_stats(uint8_t characterId, uint8_t *life, uint8_t *defense, uint8_t *ultimateThreshold);

/**
 * Handle character-specific start phase effects
 * @param gameState Pointer to game state
 * @param player Pointer to current player
 * @return true if successful, false otherwise
 */
bool handle_character_start_phase(game *gameState, player *player);

/**
 * Handle character-specific passive abilities
 * @param gameState Pointer to game state
 * @param playerId Player ID
 * @return true if successful, false otherwise
 */
bool handle_character_passive_abilities(game *gameState, int8_t playerId);

/**
 * Validate character-specific action
 * @param gameState Pointer to game state
 * @param playerId Player ID
 * @param action Action to validate
 * @return true if valid, false otherwise
 */
bool validate_character_action(game *gameState, int8_t playerId, int32_t action);

/**
 * Execute character-specific special action
 * @param gameState Pointer to game state
 * @param playerId Player ID
 * @param action Special action
 * @return true if successful, false otherwise
 */
bool execute_character_special_action(game *gameState, int8_t playerId, int32_t action);

// Character-specific functions

/**
 * Red Riding Hood (0) - Technology-enhanced abilities
 */
bool handle_red_riding_hood_abilities(game *gameState, player *player);
bool red_riding_hood_board_buffer_store(player *player, int32_t cardId);
bool red_riding_hood_board_buffer_retrieve(player *player, int32_t *cardId);

/**
 * Snow White (1) - Poison mechanics
 */
bool handle_snow_white_abilities(game *gameState, player *player);
bool snow_white_add_poison_to_deck(player *target, int32_t poisonLevel, int32_t count);
bool snow_white_shuffle_poison_to_deck(player *target);

/**
 * Sleeping Beauty (2) - Awakening mechanism
 */
bool handle_sleeping_beauty_abilities(game *gameState, player *player);
bool sleeping_beauty_gain_awaken_token(player *player, int32_t count);
bool sleeping_beauty_spend_awaken_token(player *player, int32_t count);
bool sleeping_beauty_toggle_awaken_state(player *player);

/**
 * Alice (3) - Identity switching
 */
bool handle_alice_abilities(game *gameState, player *player);
bool alice_switch_identity(player *player, uint8_t newIdentity);
bool alice_get_identity_bonus(player *player, int32_t *attackBonus, int32_t *defenseBonus, int32_t *moveBonus);

/**
 * Mulan (4) - Ki resource management
 */
bool handle_mulan_abilities(game *gameState, player *player);
bool mulan_gain_ki_token(player *player, int32_t count);
bool mulan_spend_ki_token(player *player, int32_t count);
bool mulan_reduce_damage_gain_ki(player *player, int32_t damage);

/**
 * Princess Kaguya (5) - Defense mechanics
 */
bool handle_kaguya_abilities(game *gameState, player *player);
bool kaguya_use_defense_as_attack(player *player, bool enable);
bool kaguya_defense_based_effects(player *player);

/**
 * Little Mermaid (6) - Tentacle control
 */
bool handle_mermaid_abilities(game *gameState, player *player);
bool mermaid_place_tentacle(game *gameState, int32_t location);
bool mermaid_move_tentacle(game *gameState, int32_t tentacleIndex, int32_t newLocation);
bool mermaid_get_tentacle_positions(game *gameState, vector *positions);

/**
 * Match Girl (7) - Match card distribution
 */
bool handle_match_girl_abilities(game *gameState, player *player);
bool match_girl_distribute_match_cards(game *gameState, player *target, int32_t count);
bool match_girl_recycle_match_cards(game *gameState, player *target, int32_t count);

/**
 * Dorothy (8) - Combo system
 */
bool handle_dorothy_abilities(game *gameState, player *player);
bool dorothy_gain_combo_token(player *player, int32_t count);
bool dorothy_spend_combo_tokens(player *player, int32_t count);
bool dorothy_trigger_combo_effect(game *gameState, player *player);

/**
 * Scheherazade (9) - Supply deck control
 */
bool handle_scheherazade_abilities(game *gameState, player *player);
bool scheherazade_place_destiny_token(player *player, int32_t location, int32_t type);
bool scheherazade_move_destiny_token(player *player, int32_t tokenIndex, int32_t newLocation);
bool scheherazade_flip_token_to_red(player *player, int32_t tokenIndex);

#endif // _CHARACTER_SYSTEM_H
