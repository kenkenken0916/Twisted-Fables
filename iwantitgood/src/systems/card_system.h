#ifndef _CARD_SYSTEM_H
#define _CARD_SYSTEM_H

#include "../utils/architecture.h"

/**
 * Initialize the card system
 * @return true if successful, false otherwise
 */
bool init_card_system(void);

/**
 * Cleanup the card system
 */
void cleanup_card_system(void);

/**
 * Get card value based on card ID
 * @param cardId Card ID
 * @return Card value (level for basic cards, special values for others)
 */
int32_t get_card_value(int32_t cardId);

/**
 * Get card type from card ID
 * @param cardId Card ID
 * @return Card type (0=attack, 1=defense, 2=movement, 3=skill, 4=ultimate, 5=metamorphosis, 6=special)
 */
int32_t get_card_type(int32_t cardId);

/**
 * Get card cost for purchasing
 * @param cardId Card ID
 * @return Energy cost to purchase card
 */
int32_t get_card_cost(int32_t cardId);

/**
 * Check if card meets requirements for use
 * @param gameState Pointer to game state
 * @param cardId Card ID
 * @param playerId Player attempting to use card
 * @return true if requirements met, false otherwise
 */
bool check_card_requirements(game *gameState, int32_t cardId, int8_t playerId);

/**
 * Execute card effect
 * @param gameState Pointer to game state
 * @param cardId Card ID
 * @param playerId Player using card
 * @param basicCardValue Value of basic card used with skill (if applicable)
 * @return true if successful, false otherwise
 */
bool execute_card_effect(game *gameState, int32_t cardId, int8_t playerId, int32_t basicCardValue);

/**
 * Check if card is a basic card (attack, defense, movement, universal)
 * @param cardId Card ID
 * @return true if basic card, false otherwise
 */
bool is_basic_card(int32_t cardId);

/**
 * Check if card is a skill card
 * @param cardId Card ID
 * @return true if skill card, false otherwise
 */
bool is_skill_card(int32_t cardId);

/**
 * Check if card is an ultimate card
 * @param cardId Card ID
 * @return true if ultimate card, false otherwise
 */
bool is_ultimate_card(int32_t cardId);

/**
 * Check if card is a metamorphosis card
 * @param cardId Card ID
 * @return true if metamorphosis card, false otherwise
 */
bool is_metamorphosis_card(int32_t cardId);

/**
 * Get character that owns a specific card
 * @param cardId Card ID
 * @return Character ID (0-9), or -1 if basic/universal card
 */
int8_t get_card_owner_character(int32_t cardId);

/**
 * Get skill requirement type (attack/defense/movement)
 * @param skillCardId Skill card ID
 * @return Required basic card type (0=attack, 1=defense, 2=movement, -1=invalid)
 */
int8_t get_skill_requirement_type(int32_t skillCardId);

/**
 * Calculate damage for attack cards or skills
 * @param gameState Pointer to game state
 * @param cardId Card ID
 * @param playerId Player using card
 * @param basicCardValue Value of basic card (for skills)
 * @return Calculated damage
 */
int32_t calculate_damage(game *gameState, int32_t cardId, int8_t playerId, int32_t basicCardValue);

/**
 * Calculate defense for defense cards or skills
 * @param gameState Pointer to game state
 * @param cardId Card ID
 * @param playerId Player using card
 * @param basicCardValue Value of basic card (for skills)
 * @return Calculated defense
 */
int32_t calculate_defense(game *gameState, int32_t cardId, int8_t playerId, int32_t basicCardValue);

/**
 * Calculate movement for movement cards or skills
 * @param gameState Pointer to game state
 * @param cardId Card ID
 * @param playerId Player using card
 * @param basicCardValue Value of basic card (for skills)
 * @return Calculated movement
 */
int32_t calculate_movement(game *gameState, int32_t cardId, int8_t playerId, int32_t basicCardValue);

/**
 * Get card range for skills
 * @param cardId Card ID
 * @return Range of the card effect
 */
int32_t get_card_range(int32_t cardId);

/**
 * Get card name for display
 * @param cardId Card ID
 * @return Card name string
 */
const char *get_card_name(int32_t cardId);

/**
 * Execute basic card effect
 * @param gameState Pointer to game state
 * @param cardId Card ID
 * @param playerId Player ID
 * @return true if successful, false otherwise
 */
bool execute_basic_card_effect(game *gameState, int32_t cardId, int8_t playerId);

/**
 * Execute skill card effect
 * @param gameState Pointer to game state
 * @param cardId Card ID
 * @param playerId Player ID
 * @param basicCardValue Value of basic card
 * @return true if successful, false otherwise
 */
bool execute_skill_card_effect(game *gameState, int32_t cardId, int8_t playerId, int32_t basicCardValue);

/**
 * Execute ultimate card effect
 * @param gameState Pointer to game state
 * @param cardId Card ID
 * @param playerId Player ID
 * @return true if successful, false otherwise
 */
bool execute_ultimate_card_effect(game *gameState, int32_t cardId, int8_t playerId);

// Red Riding Hood skill functions
bool execute_quick_shot(game *gameState, int8_t playerId, int32_t basicCardValue);
bool execute_precision_shot(game *gameState, int8_t playerId, int32_t basicCardValue);
bool execute_lethal_shot(game *gameState, int8_t playerId, int32_t basicCardValue);
bool execute_energy_shield(game *gameState, int8_t playerId, int32_t basicCardValue);
bool execute_current_shield(game *gameState, int8_t playerId, int32_t basicCardValue);
bool execute_ultimate_shield(game *gameState, int8_t playerId, int32_t basicCardValue);
bool execute_ballistic_jet(game *gameState, int8_t playerId, int32_t basicCardValue);
bool execute_power_jet(game *gameState, int8_t playerId, int32_t basicCardValue);
bool execute_rage_jet(game *gameState, int8_t playerId, int32_t basicCardValue);

// Ultimate skill functions
bool execute_wolfs_devour(game *gameState, int8_t playerId);
bool execute_system_invasion(game *gameState, int8_t playerId);
bool execute_revenge_rain(game *gameState, int8_t playerId);

#endif // _CARD_SYSTEM_H
