#include "utils.h"
#include "debug_log.h"
#include <stdlib.h>

static bool random_initialized = false;

void shuffle_vector(vector *vec)
{
    if (!vec || vec->SIZE <= 1)
    {
        return;
    }

    if (!random_initialized)
    {
        init_random_seed();
    }

    // Fisher-Yates shuffle
    for (uint32_t i = vec->SIZE - 1; i > 0; i--)
    {
        uint32_t j = rand() % (i + 1);

        // Swap elements
        int32_t temp = vec->array[i];
        vec->array[i] = vec->array[j];
        vec->array[j] = temp;
    }
}

int32_t get_random_range(int32_t min, int32_t max)
{
    if (!random_initialized)
    {
        init_random_seed();
    }

    if (min > max)
    {
        int32_t temp = min;
        min = max;
        max = temp;
    }

    return min + (rand() % (max - min + 1));
}

int32_t calculate_distance(int32_t pos1, int32_t pos2)
{
    return abs(pos1 - pos2);
}

bool is_valid_position(int32_t position)
{
    return position >= 1 && position <= 9;
}

bool is_within_range(int32_t playerPos, int32_t targetPos, int32_t range)
{
    if (!is_valid_position(playerPos) || !is_valid_position(targetPos))
    {
        return false;
    }

    return calculate_distance(playerPos, targetPos) <= range;
}

int32_t apply_damage_to_player(player *player, int32_t damage)
{
    if (!player || damage <= 0)
    {
        return 0;
    }

    int32_t originalLife = player->life;

    // Damage reduces defense first, then life
    if (player->defense > 0)
    {
        int32_t defenseReduction = (damage > player->defense) ? player->defense : damage;
        player->defense -= defenseReduction;
        damage -= defenseReduction;
    }

    // Remaining damage reduces life
    if (damage > 0)
    {
        if (damage >= player->life)
        {
            player->life = 0;
        }
        else
        {
            player->life -= damage;
        }
    }

    int32_t lifeDamage = originalLife - player->life;

    DEBUG_LOG("Applied %d damage to player (defense reduction + life damage: %d)",
              damage + (originalLife - player->life - lifeDamage), lifeDamage);

    return lifeDamage;
}

int32_t heal_player(player *player, int32_t healAmount)
{
    if (!player || healAmount <= 0)
    {
        return 0;
    }

    int32_t originalLife = player->life;
    player->life += healAmount;

    if (player->life > player->maxlife)
    {
        player->life = player->maxlife;
    }

    int32_t actualHeal = player->life - originalLife;

    DEBUG_LOG("Healed player for %d life (%d -> %d)", actualHeal, originalLife, player->life);

    return actualHeal;
}

bool move_player(game *gameState, int8_t playerId, int32_t newPosition)
{
    if (!gameState || playerId < 0 || playerId >= 4)
    {
        ERROR_LOG("Invalid parameters for player movement");
        return false;
    }

    if (!is_valid_position(newPosition))
    {
        ERROR_LOG("Invalid position for movement: %d", newPosition);
        return false;
    }

    player *p = &gameState->players[playerId];

    // Check if position is occupied (players cannot overlap)
    if (is_position_occupied(gameState, newPosition, playerId))
    {
        WARNING_LOG("Position %d is occupied, cannot move player %d", newPosition, playerId);
        return false;
    }

    int32_t oldPosition = p->locate[0];
    p->locate[0] = newPosition;

    DEBUG_LOG("Player %d moved from position %d to %d", playerId, oldPosition, newPosition);

    return true;
}

bool knockback_player(game *gameState, int8_t playerId, int32_t distance, int8_t direction)
{
    if (!gameState || playerId < 0 || playerId >= 4 || distance <= 0)
    {
        ERROR_LOG("Invalid parameters for knockback");
        return false;
    }

    player *p = &gameState->players[playerId];
    int32_t currentPos = p->locate[0];
    int32_t targetPos;

    if (direction < 0)
    {
        // Knockback left
        targetPos = currentPos - distance;
    }
    else
    {
        // Knockback right
        targetPos = currentPos + distance;
    }

    // Find the furthest valid position in the knockback direction
    int32_t finalPos = currentPos;
    int32_t step = (direction < 0) ? -1 : 1;

    for (int32_t pos = currentPos + step;
         ((direction < 0 && pos >= targetPos) || (direction > 0 && pos <= targetPos)) && is_valid_position(pos);
         pos += step)
    {

        if (!is_position_occupied(gameState, pos, playerId))
        {
            finalPos = pos;
        }
        else
        {
            break; // Stop if position is occupied
        }
    }

    if (finalPos != currentPos)
    {
        p->locate[0] = finalPos;
        DEBUG_LOG("Player %d knocked back from %d to %d (distance %d)",
                  playerId, currentPos, finalPos, abs(finalPos - currentPos));
    }

    return true;
}

int8_t find_target_in_range(game *gameState, int8_t attackerId, int32_t range)
{
    if (!gameState || attackerId < 0 || attackerId >= 4)
    {
        ERROR_LOG("Invalid parameters for target finding");
        return -1;
    }

    player *attacker = &gameState->players[attackerId];
    int32_t attackerPos = attacker->locate[0];
    int32_t attackerTrack = attacker->locate[1];

    int playerCount = (gameState->playerMode == 0) ? 2 : 4;

    for (int8_t i = 0; i < playerCount; i++)
    {
        if (i == attackerId)
            continue; // Skip self

        player *target = &gameState->players[i];

        // In 2v2 mode, cannot attack between tracks
        if (gameState->playerMode == 1 && target->locate[1] != attackerTrack)
        {
            continue;
        }

        // Check if target is within range and alive
        if (target->life > 0 && is_within_range(attackerPos, target->locate[0], range))
        {
            return i;
        }
    }

    return -1; // No valid target found
}

int32_t get_players_in_range(game *gameState, int8_t playerId, int32_t range, vector *targets)
{
    if (!gameState || playerId < 0 || playerId >= 4 || !targets)
    {
        ERROR_LOG("Invalid parameters for range checking");
        return 0;
    }

    clearVector(targets);

    player *sourcePlayer = &gameState->players[playerId];
    int32_t sourcePos = sourcePlayer->locate[0];
    int32_t sourceTrack = sourcePlayer->locate[1];

    int playerCount = (gameState->playerMode == 0) ? 2 : 4;
    int32_t count = 0;

    for (int8_t i = 0; i < playerCount; i++)
    {
        if (i == playerId)
            continue; // Skip self

        player *target = &gameState->players[i];

        // In 2v2 mode, cannot target between tracks
        if (gameState->playerMode == 1 && target->locate[1] != sourceTrack)
        {
            continue;
        }

        // Check if target is within range and alive
        if (target->life > 0 && is_within_range(sourcePos, target->locate[0], range))
        {
            pushbackVector(targets, i);
            count++;
        }
    }

    return count;
}

bool is_position_occupied(game *gameState, int32_t position, int8_t excludePlayer)
{
    if (!gameState || !is_valid_position(position))
    {
        return true; // Invalid positions are considered occupied
    }

    int playerCount = (gameState->playerMode == 0) ? 2 : 4;

    for (int8_t i = 0; i < playerCount; i++)
    {
        if (i == excludePlayer)
            continue;

        if (gameState->players[i].locate[0] == position && gameState->players[i].life > 0)
        {
            return true;
        }
    }

    return false;
}

int32_t get_next_available_position(game *gameState, int32_t startPos, int8_t direction, int8_t excludePlayer)
{
    if (!gameState || !is_valid_position(startPos))
    {
        return -1;
    }

    int32_t step = (direction < 0) ? -1 : 1;
    int32_t pos = startPos + step;

    while (is_valid_position(pos))
    {
        if (!is_position_occupied(gameState, pos, excludePlayer))
        {
            return pos;
        }
        pos += step;
    }

    return -1; // No available position found
}

int32_t clamp_value(int32_t value, int32_t min, int32_t max)
{
    if (value < min)
        return min;
    if (value > max)
        return max;
    return value;
}

void init_random_seed(void)
{
    if (!random_initialized)
    {
        srand((unsigned int)time(NULL));
        random_initialized = true;
        DEBUG_LOG("Random seed initialized");
    }
}

bool is_passing_through(int32_t start_pos, int32_t target_pos, int32_t move_distance)
{
    // Moving right
    if (start_pos < target_pos)
    {
        return target_pos <= start_pos + move_distance;
    }
    // Moving left
    else if (start_pos > target_pos)
    {
        return target_pos >= start_pos - move_distance;
    }
    return false;
}
