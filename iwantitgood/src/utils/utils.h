#ifndef _UTILS_H
#define _UTILS_H

#include "architecture.h"
#include <time.h>

/**
 * Shuffle a vector using Fisher-Yates algorithm
 * @param vec Pointer to vector to shuffle
 */
void shuffle_vector(vector *vec);

/**
 * Get random number between min and max (inclusive)
 * @param min Minimum value
 * @param max Maximum value
 * @return Random number in range [min, max]
 */
int32_t get_random_range(int32_t min, int32_t max);

/**
 * Calculate distance between two positions
 * @param pos1 First position
 * @param pos2 Second position
 * @return Distance between positions
 */
int32_t calculate_distance(int32_t pos1, int32_t pos2);

/**
 * Check if position is valid on battle track
 * @param position Position to check (1-9)
 * @return true if valid, false otherwise
 */
bool is_valid_position(int32_t position);

/**
 * Check if player is within range of target
 * @param playerPos Player position
 * @param targetPos Target position
 * @param range Attack/skill range
 * @return true if within range, false otherwise
 */
bool is_within_range(int32_t playerPos, int32_t targetPos, int32_t range);

/**
 * Apply damage to player (defense first, then life)
 * @param player Pointer to player
 * @param damage Damage amount
 * @return Actual damage dealt to life
 */
int32_t apply_damage_to_player(player *player, int32_t damage);

/**
 * Heal player (cannot exceed max life)
 * @param player Pointer to player
 * @param healAmount Amount to heal
 * @return Actual amount healed
 */
int32_t heal_player(player *player, int32_t healAmount);

/**
 * Move player to new position
 * @param gameState Pointer to game state
 * @param playerId Player ID
 * @param newPosition New position (1-9)
 * @return true if move successful, false otherwise
 */
bool move_player(game *gameState, int8_t playerId, int32_t newPosition);

/**
 * Knockback player by specified distance
 * @param gameState Pointer to game state
 * @param playerId Player ID
 * @param distance Knockback distance
 * @param direction Direction (-1 for left, 1 for right)
 * @return true if knockback successful, false otherwise
 */
bool knockback_player(game *gameState, int8_t playerId, int32_t distance, int8_t direction);

/**
 * Find target player within range
 * @param gameState Pointer to game state
 * @param attackerId Attacker player ID
 * @param range Attack range
 * @return Target player ID, or -1 if no valid target
 */
int8_t find_target_in_range(game *gameState, int8_t attackerId, int32_t range);

/**
 * Get all players within range
 * @param gameState Pointer to game state
 * @param playerId Source player ID
 * @param range Range to check
 * @param targets Pointer to vector to store target IDs
 * @return Number of targets found
 */
int32_t get_players_in_range(game *gameState, int8_t playerId, int32_t range, vector *targets);

/**
 * Check if position is occupied by another player
 * @param gameState Pointer to game state
 * @param position Position to check
 * @param excludePlayer Player ID to exclude from check (-1 for none)
 * @return true if occupied, false otherwise
 */
bool is_position_occupied(game *gameState, int32_t position, int8_t excludePlayer);

/**
 * Get next available position in direction
 * @param gameState Pointer to game state
 * @param startPos Starting position
 * @param direction Direction (-1 for left, 1 for right)
 * @param excludePlayer Player to exclude from collision check
 * @return Next available position, or -1 if none
 */
int32_t get_next_available_position(game *gameState, int32_t startPos, int8_t direction, int8_t excludePlayer);

/**
 * Clamp value to specified range
 * @param value Value to clamp
 * @param min Minimum value
 * @param max Maximum value
 * @return Clamped value
 */
int32_t clamp_value(int32_t value, int32_t min, int32_t max);

/**
 * Initialize random seed
 */
void init_random_seed(void);

/**
 * Check if moving from start_pos by move_distance will pass through target_pos
 * @param start_pos Starting position
 * @param target_pos Target's position
 * @param move_distance Movement distance
 * @return true if movement passes through target, false otherwise
 */
bool is_passing_through(int32_t start_pos, int32_t target_pos, int32_t move_distance);

#endif // _UTILS_H
