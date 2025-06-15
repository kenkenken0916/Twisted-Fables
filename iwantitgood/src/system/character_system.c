#include "character_system.h"
#include "../utils/debug_log.h"
#include "../utils/utils.h"

// Character names
static const char *character_names[] = {
    "Red Riding Hood", // 0
    "Snow White",      // 1
    "Sleeping Beauty", // 2
    "Alice",           // 3
    "Mulan",           // 4
    "Princess Kaguya", // 5
    "Little Mermaid",  // 6
    "Match Girl",      // 7
    "Dorothy",         // 8
    "Scheherazade"     // 9
};

// Character base stats
typedef struct
{
    uint8_t maxLife;
    uint8_t maxDefense;
    uint8_t ultimateThreshold;
} CharacterStats;

static CharacterStats character_stats[] = {
    {30, 6, 15}, // Red Riding Hood
    {34, 6, 17}, // Snow White
    {42, 6, 21}, // Sleeping Beauty
    {32, 6, 16}, // Alice
    {34, 3, 17}, // Mulan
    {32, 6, 16}, // Princess Kaguya
    {36, 3, 18}, // Little Mermaid
    {36, 6, 18}, // Match Girl
    {40, 6, 20}, // Dorothy
    {36, 6, 18}  // Scheherazade
};

bool init_character_system(void)
{
    CHAR_LOG(LOG_DEBUG, "Initializing character system");

    // Initialize character system data structures
    // For now, just log initialization

    CHAR_LOG(LOG_INFO, "Character system initialized successfully");
    return true;
}

void cleanup_character_system(void)
{
    CHAR_LOG(LOG_DEBUG, "Cleaning up character system");

    // Cleanup character system resources
    // For now, just log cleanup

    CHAR_LOG(LOG_DEBUG, "Character system cleanup completed");
}

const char *get_character_name(uint8_t characterId)
{
    if (characterId <= 9)
    { // characterId is uint8_t, no need to check >= 0
        return character_names[characterId];
    }
    return "Unknown Character";
}

bool get_character_stats(uint8_t characterId, uint8_t *life, uint8_t *defense, uint8_t *ultimateThreshold)
{
    if (characterId > 9 || !life || !defense || !ultimateThreshold)
    {
        ERROR_LOG("Invalid parameters for character stats");
        return false;
    }

    *life = character_stats[characterId].maxLife;
    *defense = character_stats[characterId].maxDefense;
    *ultimateThreshold = character_stats[characterId].ultimateThreshold;

    return true;
}

bool handle_character_start_phase(game *gameState, player *player)
{
    if (!gameState || !player)
    {
        ERROR_LOG("Invalid parameters for character start phase");
        return false;
    }

    CHAR_LOG(LOG_DEBUG, "Handling start phase for character %d", player->character);

    switch (player->character)
    {
    case 0: // Red Riding Hood - Check board buffer
        return handle_red_riding_hood_abilities(gameState, player);
    case 1: // Snow White - Apply poison effects
        return handle_snow_white_abilities(gameState, player);
    case 2: // Sleeping Beauty - Handle awakening
        return handle_sleeping_beauty_abilities(gameState, player);
    case 3: // Alice - Required identity switch
        return handle_alice_abilities(gameState, player);
    case 4: // Mulan - Ki management
        return handle_mulan_abilities(gameState, player);
    case 5: // Princess Kaguya - Defense mechanics
        return handle_kaguya_abilities(gameState, player);
    case 6: // Little Mermaid - Tentacle control
        return handle_mermaid_abilities(gameState, player);
    case 7: // Match Girl - Match card effects
        return handle_match_girl_abilities(gameState, player);
    case 8: // Dorothy - Combo management
        return handle_dorothy_abilities(gameState, player);
    case 9: // Scheherazade - Destiny token effects
        return handle_scheherazade_abilities(gameState, player);
    default:
        WARNING_LOG("Unknown character ID: %d", player->character);
        break;
    }

    return true;
}

bool handle_character_passive_abilities(game *gameState, int8_t playerId)
{
    if (!gameState || playerId < 0 || playerId >= 4)
    {
        ERROR_LOG("Invalid parameters for character passive abilities");
        return false;
    }

    player *p = &gameState->players[playerId];

    // Handle passive abilities that are always active
    switch (p->character)
    {
    case 3: // Alice - Identity bonuses
    {
        int32_t atkBonus, defBonus, moveBonus;
        if (alice_get_identity_bonus(p, &atkBonus, &defBonus, &moveBonus))
        {
            // Apply bonuses to current actions
            gameState->nowATK += atkBonus;
            gameState->nowDEF += defBonus;
            gameState->nowMOV += moveBonus;
        }
    }
    break;
    case 5: // Princess Kaguya - Defense-based effects
        kaguya_defense_based_effects(p);
        break;
    default:
        // No passive abilities or not implemented yet
        break;
    }

    return true;
}

bool handle_red_riding_hood_abilities(game *gameState, player *player)
{
    (void)gameState; // Suppress unused parameter warning

    // Check board buffer
    for (int i = 0; i < 3; i++)
    {
        if (player->redHood.saveCard[i] != -1)
        {
            CHAR_LOG(LOG_DEBUG, "Board buffer contains card %d in slot %d",
                     player->redHood.saveCard[i], i);
        }
    }

    return true;
}

bool handle_snow_white_abilities(game *gameState, player *player)
{
    (void)gameState;

    // Check for poison cards in target's deck
    if (player->snowWhite.remindPosion.SIZE > 0)
    {
        CHAR_LOG(LOG_DEBUG, "Snow White has %d poison cards to distribute",
                 player->snowWhite.remindPosion.SIZE);
    }

    return true;
}

bool handle_sleeping_beauty_abilities(game *gameState, player *player)
{
    if (!gameState || !player || player->character != 2)
        return false;

    // Check awaken state
    if (player->sleepingBeauty.AWAKEN)
    {
        // Apply awakened effects
        if (player->sleepingBeauty.atkRiseTime > 0)
        {
            player->sleepingBeauty.atkRiseTime--;
            if (player->sleepingBeauty.atkRiseTime == 0)
            {
                player->sleepingBeauty.atkRise = 0;
            }
        }
    }

    return true;
}

bool handle_alice_abilities(game *gameState, player *player)
{
    if (!gameState || !player || player->character != 3)
    {
        return false;
    }

    // Identity must be one of: 1 (Queen), 2 (Hatter), 3 (Cat), 0 (none)
    if (player->alice.identity > 3)
    {
        ERROR_LOG("Invalid Alice identity: %d", player->alice.identity);
        player->alice.identity = 0;
    }

    // Reset riseBasic/restartTurn at start of each turn
    player->alice.riseBasic = 0;
    player->alice.havedrestart = 0;

    return true;
}

bool alice_switch_identity(player *player, uint8_t newIdentity)
{
    if (!player || player->character != 3)
    {
        ERROR_LOG("Invalid player for Alice identity switch");
        return false;
    }

    if (newIdentity > 3)
    {
        ERROR_LOG("Invalid identity value: %d", newIdentity);
        return false;
    }

    CHAR_LOG(LOG_DEBUG, "Alice switching identity from %d to %d",
             player->alice.identity, newIdentity);

    // Update identity
    player->alice.identity = newIdentity;

    return true;
}

bool alice_get_identity_bonus(player *player, int32_t *attackBonus, int32_t *defenseBonus, int32_t *moveBonus)
{
    if (!player || player->character != 3 || !attackBonus || !defenseBonus || !moveBonus)
    {
        ERROR_LOG("Invalid parameters for identity bonus check");
        return false;
    }

    *attackBonus = 0;
    *defenseBonus = 0;
    *moveBonus = 0;

    // Apply identity-specific bonuses
    switch (player->alice.identity)
    {
    case 1: // Queen of Hearts
        *attackBonus = 1;
        break;
    case 2: // Mad Hatter
        *defenseBonus = 1;
        break;
    case 3: // Cheshire Cat
        *moveBonus = 1;
        break;
    default:
        break;
    }

    return true;
}

bool handle_mulan_abilities(game *gameState, player *player)
{
    if (!gameState || !player || player->character != 4)
        return false;

    // Process Ki token effects
    if (player->mulan.KI_TOKEN > 12)
    {
        player->mulan.KI_TOKEN = 12; // Cap at 12 tokens
    }

    // Handle extra card draw if applicable
    if (player->mulan.extraDraw)
    {
        draw_cards(player, player->mulan.extraDraw);
        player->mulan.extraDraw = 0;
    }

    return true;
}

bool handle_kaguya_abilities(game *gameState, player *player)
{
    if (!gameState || !player || player->character != 5)
        return false;

    // Reset defense-as-attack flag at start of turn
    player->kaguya.useDefenseAsATK = 0;

    return true;
}

bool handle_mermaid_abilities(game *gameState, player *player)
{
    if (!gameState || !player || player->character != 6)
        return false;

    // Update tentacle positions and effects
    // Tentacles are stored in gameState->tentacle_TOKEN_locate
    return true;
}

bool handle_match_girl_abilities(game *gameState, player *player)
{
    if (!gameState || !player || player->character != 7)
        return false;

    // Process match card distribution
    if (player->matchGirl.remindMatch > 0)
    {
        CHAR_LOG(LOG_DEBUG, "Match Girl has %d match cards to distribute",
                 player->matchGirl.remindMatch);
    }

    return true;
}

bool handle_dorothy_abilities(game *gameState, player *player)
{
    if (!gameState || !player || player->character != 8)
        return false;

    // Cap combo tokens at 12
    if (player->dorothy.COMBO_TOKEN > 12)
    {
        player->dorothy.COMBO_TOKEN = 12;
    }

    return true;
}

bool handle_scheherazade_abilities(game *gameState, player *player)
{
    if (!gameState || !player || player->character != 9)
        return false;

    // Process destiny token effects
    if (player->scheherazade.destiny_TOKEN_locate.SIZE > 6)
    {
        while (player->scheherazade.destiny_TOKEN_locate.SIZE > 6)
        {
            popbackVector(&player->scheherazade.destiny_TOKEN_locate);
            popbackVector(&player->scheherazade.destiny_TOKEN_type);
        }
    }

    return true;
}
