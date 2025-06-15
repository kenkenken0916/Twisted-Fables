#include "card_system.h"
#include "../utils/debug_log.h"

// Card data structures
typedef struct
{
    int32_t id;
    const char *name;
    int32_t type;
    int32_t value;
    int32_t cost;
    int32_t range;
    int8_t character;   // -1 for basic/universal cards
    int8_t requirement; // For skills: 0=attack, 1=defense, 2=movement
} CardData;

// Basic cards (IDs 1-10)
static CardData basic_cards[] = {
    {1, "Level 1 Attack", 0, 1, 2, 1, -1, -1},
    {2, "Level 2 Attack", 0, 2, 4, 1, -1, -1},
    {3, "Level 3 Attack", 0, 3, 6, 1, -1, -1},
    {4, "Level 1 Defense", 1, 1, 2, 0, -1, -1},
    {5, "Level 2 Defense", 1, 2, 4, 0, -1, -1},
    {6, "Level 3 Defense", 1, 3, 6, 0, -1, -1},
    {7, "Level 1 Movement", 2, 1, 2, 0, -1, -1},
    {8, "Level 2 Movement", 2, 2, 4, 0, -1, -1},
    {9, "Level 3 Movement", 2, 3, 6, 0, -1, -1},
    {10, "Universal Card", 3, 1, 2, 0, -1, -1}};

// Red Riding Hood cards (IDs 11-22, 135-138)
static CardData red_riding_hood_cards[] = {
    // Attack Skills
    {11, "Quick Shot", 3, 1, 0, 1, 0, 0},
    {12, "Precision Shot", 3, 2, 2, 2, 0, 0},
    {13, "Lethal Shot", 3, 3, 4, 3, 0, 0},
    // Defense Skills
    {14, "Energy Shield", 3, 1, 0, 1, 0, 1},
    {15, "Current Shield", 3, 2, 2, 2, 0, 1},
    {16, "Ultimate Shield", 3, 3, 4, 3, 0, 1},
    // Movement Skills
    {17, "Ballistic Jet", 3, 1, 0, 1, 0, 2},
    {18, "Power Jet", 3, 2, 2, 2, 0, 2},
    {19, "Rage Jet", 3, 3, 4, 3, 0, 2},
    // Ultimate Cards
    {20, "Wolf's Devour", 4, 0, 0, 0, 0, -1},
    {21, "System Invasion", 4, 0, 0, 0, 0, -1},
    {22, "Revenge Rain", 4, 3, 0, 3, 0, -1},
    // Metamorphosis Cards
    {135, "Overload Burn", 5, 0, 0, 0, 0, -1},
    {136, "Hood System", 5, 0, 0, 0, 0, -1},
    {137, "Mutation Sense", 5, 0, 0, 0, 0, -1},
    {138, "Board Buffer", 5, 0, 0, 0, 0, -1}};

// Snow White cards (IDs 23-34, 131-134, 139-142)
static CardData snow_white_cards[] = {
    // Attack Skills
    {23, "Poison Blade", 3, 1, 0, 1, 1, 0},
    {24, "Toxic Slash", 3, 2, 2, 2, 1, 0},
    {25, "Venom Strike", 3, 3, 4, 3, 1, 0},
    // Defense Skills
    {26, "Mirror Shield", 3, 1, 0, 1, 1, 1},
    {27, "Crystal Shield", 3, 2, 2, 2, 1, 1},
    {28, "Prism Shield", 3, 3, 4, 3, 1, 1},
    // Movement Skills
    {29, "Shadow Step", 3, 1, 0, 1, 1, 2},
    {30, "Phase Step", 3, 2, 2, 2, 1, 2},
    {31, "Specter Step", 3, 3, 4, 3, 1, 2},
    // Ultimate Cards
    {32, "Queen's Decree", 4, 0, 0, 0, 0, -1},
    {33, "Poison Mist", 4, 0, 0, 0, 0, -1},
    {34, "Eternal Slumber", 4, 3, 0, 3, 0, -1},
    // Metamorphosis Cards
    {139, "Overload Burn", 5, 0, 0, 0, 0, -1},
    {140, "Hood System", 5, 0, 0, 0, 0, -1},
    {141, "Mutation Sense", 5, 0, 0, 0, 0, -1},
    {142, "Board Buffer", 5, 0, 0, 0, 0, -1}};

// Sleeping Beauty cards (IDs 35-46, 143-146)
static CardData sleeping_beauty_cards[] = {
    // Attack Skills
    {35, "Dream Slash", 3, 1, 0, 1, 2, 0},
    {36, "Nightmare Curse", 3, 2, 2, 2, 2, 0},
    {37, "Thorny Rose", 3, 3, 4, 3, 2, 0},
    // Ultimate Cards
    {44, "Dream Realm", 4, 0, 0, 0, 0, -1},
    {45, "Rose Garden", 4, 0, 0, 0, 0, -1},
    {46, "Eternal Dream", 4, 3, 0, 3, 0, -1},
    // Metamorphosis Cards
    {143, "Overload Burn", 5, 0, 0, 0, 0, -1},
    {144, "Hood System", 5, 0, 0, 0, 0, -1},
    {145, "Mutation Sense", 5, 0, 0, 0, 0, -1},
    {146, "Board Buffer", 5, 0, 0, 0, 0, -1}};

// Alice cards (IDs 47-58, 147-150)
static CardData alice_cards[] = {
    // Attack Skills
    {47, "Open Game", 3, 1, 0, 1, 3, 0},
    {48, "Turn Game", 3, 2, 2, 2, 3, 0},
    {49, "Madness", 3, 3, 4, 3, 3, 0},
    // Defense Skills
    {50, "Magic Trick", 3, 1, 0, 1, 3, 1},
    {51, "Mirror Image", 3, 2, 2, 2, 3, 1},
    {52, "Cheshire Grin", 3, 3, 4, 3, 3, 1},
    // Movement Skills
    {53, "Strange Agility", 3, 1, 0, 1, 3, 2},
    {54, "Caterpillar Crawl", 3, 2, 2, 2, 3, 2},
    {55, "March Hare Hop", 3, 3, 4, 3, 3, 2},
    // Ultimate Cards
    {56, "Endless Party", 4, 0, 0, 0, 3, -1},
    {57, "Wonderful Wonder Day", 4, 0, 0, 0, 3, -1},
    {58, "Queen of Hearts", 4, 3, 0, 3, 3, -1},
    // Metamorphosis Cards
    {147, "Overload Burn", 5, 0, 0, 0, 3, -1},
    {148, "Hood System", 5, 0, 0, 0, 3, -1},
    {149, "Mutation Sense", 5, 0, 0, 0, 3, -1},
    {150, "Board Buffer", 5, 0, 0, 0, 3, -1}};

bool init_card_system(void)
{
    CARD_LOG(LOG_DEBUG, "Initializing card system");

    // Initialize card system data structures
    // For now, just log initialization

    CARD_LOG(LOG_INFO, "Card system initialized successfully");
    return true;
}

void cleanup_card_system(void)
{
    CARD_LOG(LOG_DEBUG, "Cleaning up card system");

    // Cleanup card system resources
    // For now, just log cleanup

    CARD_LOG(LOG_DEBUG, "Card system cleanup completed");
}

int32_t get_card_value(int32_t cardId)
{
    // Basic cards (1-10)
    if (cardId >= 1 && cardId <= 10)
    {
        return basic_cards[cardId - 1].value;
    }

    // Red Riding Hood cards (11-22, 135-138)
    if ((cardId >= 11 && cardId <= 22) || (cardId >= 135 && cardId <= 138))
    {
        for (size_t i = 0; i < sizeof(red_riding_hood_cards) / sizeof(CardData); i++)
        {
            if (red_riding_hood_cards[i].id == cardId)
            {
                return red_riding_hood_cards[i].value;
            }
        }
    }

    // Snow White cards (23-34, 131-134, 139-142)
    if ((cardId >= 23 && cardId <= 34) || (cardId >= 131 && cardId <= 134) || (cardId >= 139 && cardId <= 142))
    {
        for (size_t i = 0; i < sizeof(snow_white_cards) / sizeof(CardData); i++)
        {
            if (snow_white_cards[i].id == cardId)
            {
                return snow_white_cards[i].value;
            }
        }
    }

    // Sleeping Beauty cards (35-46, 143-146)
    if ((cardId >= 35 && cardId <= 46) || (cardId >= 143 && cardId <= 146))
    {
        for (size_t i = 0; i < sizeof(sleeping_beauty_cards) / sizeof(CardData); i++)
        {
            if (sleeping_beauty_cards[i].id == cardId)
            {
                return sleeping_beauty_cards[i].value;
            }
        }
    }

    // Alice cards (47-58, 147-150)
    if ((cardId >= 47 && cardId <= 58) || (cardId >= 147 && cardId <= 150))
    {
        for (size_t i = 0; i < sizeof(alice_cards) / sizeof(CardData); i++)
        {
            if (alice_cards[i].id == cardId)
            {
                return alice_cards[i].value;
            }
        }
    }

    // For other characters, return appropriate values
    // This would be expanded for all characters

    return 0; // Default value
}

int32_t get_card_type(int32_t cardId)
{
    // ATK cards (1-3)
    if (cardId >= 1 && cardId <= 3)
    {
        return 0; // Attack type
    }

    // DEF cards (4-6)
    if (cardId >= 4 && cardId <= 6)
    {
        return 1; // Defense type
    }

    // MOV cards (7-9)
    if (cardId >= 7 && cardId <= 9)
    {
        return 2; // Movement type
    }

    // General cards (10)
    if (cardId == 10)
    {
        return 3; // General type
    }

    // Character skill cards ranges
    if (cardId >= 11 && cardId <= 130)
    {
        // Determine the character
        int32_t charId = (cardId - 11) / 12; // Each character has 12 cards

        // Get the card subtype within character's cards (0-11)
        int32_t cardSubtype = (cardId - 11) % 12;

        // First 4 cards of each character are attack skills
        if (cardSubtype < 4)
            return 0; // Attack
        // Next 4 cards are defense skills
        if (cardSubtype < 8)
            return 1; // Defense
        // Last 4 cards are movement skills
        return 2; // Movement
    }

    // Special cards (131-134)
    if (cardId >= 131 && cardId <= 134)
    {
        return 4; // Special type
    }

    // Metamorphosis cards (135-138)
    if (cardId >= 135 && cardId <= 138)
    {
        return 5; // Metamorphosis type
    }
}
}
}

// Snow White cards
if ((cardId >= 23 && cardId <= 34) || (cardId >= 131 && cardId <= 134) || (cardId >= 139 && cardId <= 142))
{
    for (size_t i = 0; i < sizeof(snow_white_cards) / sizeof(CardData); i++)
    {
        if (snow_white_cards[i].id == cardId)
        {
            return snow_white_cards[i].type;
        }
    }
}

// Sleeping Beauty cards
if ((cardId >= 35 && cardId <= 46) || (cardId >= 143 && cardId <= 146))
{
    for (size_t i = 0; i < sizeof(sleeping_beauty_cards) / sizeof(CardData); i++)
    {
        if (sleeping_beauty_cards[i].id == cardId)
        {
            return sleeping_beauty_cards[i].type;
        }
    }
}

// Alice cards
if ((cardId >= 47 && cardId <= 58) || (cardId >= 147 && cardId <= 150))
{
    for (size_t i = 0; i < sizeof(alice_cards) / sizeof(CardData); i++)
    {
        if (alice_cards[i].id == cardId)
        {
            return alice_cards[i].type;
        }
    }
}

// Determine type by ID range for other characters
if (cardId >= 23 && cardId <= 130)
{
    // Skill cards
    return 3;
}

if (cardId >= 131 && cardId <= 134)
{
    // Special cards (Poison, Match)
    return 6;
}

if (cardId >= 135 && cardId <= 176)
{
    // Metamorphosis cards
    return 5;
}

return -1; // Invalid card
}

int32_t get_card_cost(int32_t cardId)
{
    // Basic cards
    if (cardId >= 1 && cardId <= 10)
    {
        return basic_cards[cardId - 1].cost;
    }

    // Character-specific cards
    if ((cardId >= 11 && cardId <= 22) || (cardId >= 135 && cardId <= 138))
    {
        for (size_t i = 0; i < sizeof(red_riding_hood_cards) / sizeof(CardData); i++)
        {
            if (red_riding_hood_cards[i].id == cardId)
            {
                return red_riding_hood_cards[i].cost;
            }
        }
    }

    // Snow White cards
    if ((cardId >= 23 && cardId <= 34) || (cardId >= 131 && cardId <= 134) || (cardId >= 139 && cardId <= 142))
    {
        for (size_t i = 0; i < sizeof(snow_white_cards) / sizeof(CardData); i++)
        {
            if (snow_white_cards[i].id == cardId)
            {
                return snow_white_cards[i].cost;
            }
        }
    }

    // Sleeping Beauty cards
    if ((cardId >= 35 && cardId <= 46) || (cardId >= 143 && cardId <= 146))
    {
        for (size_t i = 0; i < sizeof(sleeping_beauty_cards) / sizeof(CardData); i++)
        {
            if (sleeping_beauty_cards[i].id == cardId)
            {
                return sleeping_beauty_cards[i].cost;
            }
        }
    }

    // Alice cards
    if ((cardId >= 47 && cardId <= 58) || (cardId >= 147 && cardId <= 150))
    {
        for (size_t i = 0; i < sizeof(alice_cards) / sizeof(CardData); i++)
        {
            if (alice_cards[i].id == cardId)
            {
                return alice_cards[i].cost;
            }
        }
    }

    // Default costs for other character cards
    // Level 1 skills: 0, Level 2 skills: 2, Level 3 skills: 4
    if (cardId >= 23 && cardId <= 130)
    {
        // Determine skill level by position in character's skill set
        int skillIndex = (cardId - 11) % 9; // 9 skills per character
        if (skillIndex < 3)
            return 0; // Level 1 skills
        if (skillIndex < 6)
            return 2; // Level 2 skills
        return 4;     // Level 3 skills
    }

    return 0; // Default cost
}

bool check_card_requirements(game *gameState, int32_t cardId, int8_t playerId)
{
    if (!gameState || playerId < 0 || playerId >= 4)
    {
        ERROR_LOG("Invalid parameters for card requirements check");
        return false;
    }

    player *p = &gameState->players[playerId];

    // Basic cards have no special requirements
    if (is_basic_card(cardId))
    {
        return true;
    }

    // Ultimate cards require life <= threshold
    if (is_ultimate_card(cardId))
    {
        return p->life <= p->specialGate;
    }

    // Skill cards require character ownership and energy
    if (is_skill_card(cardId))
    {
        int8_t cardOwner = get_card_owner_character(cardId);
        if (cardOwner != p->character && cardOwner != -1)
        {
            return false; // Wrong character
        }

        int32_t cost = get_card_cost(cardId);
        if (p->energy < cost)
        {
            return false; // Not enough energy
        }

        return true;
    }

    return true;
}

bool execute_card_effect(game *gameState, int32_t cardId, int8_t playerId, int32_t basicCardValue)
{
    if (!gameState || playerId < 0 || playerId >= 4)
    {
        ERROR_LOG("Invalid parameters for card effect execution");
        return false;
    }

    CARD_LOG(LOG_DEBUG, "Executing card %d for player %d", cardId, playerId);

    // For now, implement basic effects
    // This would be expanded with full card implementations

    if (is_basic_card(cardId))
    {
        return execute_basic_card_effect(gameState, cardId, playerId);
    }

    if (is_skill_card(cardId))
    {
        return execute_skill_card_effect(gameState, cardId, playerId, basicCardValue);
    }

    if (is_ultimate_card(cardId))
    {
        return execute_ultimate_card_effect(gameState, cardId, playerId);
    }

    return true;
}

bool is_basic_card(int32_t cardId)
{
    return cardId >= 1 && cardId <= 10;
}

bool is_skill_card(int32_t cardId)
{
    return cardId >= 11 && cardId <= 130;
}

bool is_ultimate_card(int32_t cardId)
{
    // Ultimate cards are within skill ranges but have special IDs
    // For Red Riding Hood: 20-22
    if (cardId >= 20 && cardId <= 22)
        return true;
    if (cardId >= 32 && cardId <= 34)
        return true; // Snow White
    if (cardId >= 44 && cardId <= 46)
        return true; // Sleeping Beauty
    // Add other characters...

    return false;
}

bool is_metamorphosis_card(int32_t cardId)
{
    return cardId >= 135 && cardId <= 176;
}

int8_t get_card_owner_character(int32_t cardId)
{
    if (is_basic_card(cardId))
    {
        return -1; // Universal cards
    }

    // Red Riding Hood: 11-22, 135-138
    if ((cardId >= 11 && cardId <= 22) || (cardId >= 135 && cardId <= 138))
    {
        return 0;
    }

    // Snow White: 23-34, 131-133, 139-142
    if ((cardId >= 23 && cardId <= 34) || (cardId >= 131 && cardId <= 133) || (cardId >= 139 && cardId <= 142))
    {
        return 1;
    }

    // Sleeping Beauty: 35-46, 143-146
    if ((cardId >= 35 && cardId <= 46) || (cardId >= 143 && cardId <= 146))
    {
        return 2;
    }

    // Alice: 47-58, 147-150
    if ((cardId >= 47 && cardId <= 58) || (cardId >= 147 && cardId <= 150))
    {
        return 3;
    }

    // Other characters would be added here...

    return -1; // Unknown
}

int8_t get_skill_requirement_type(int32_t skillCardId)
{
    // Red Riding Hood skills
    if (skillCardId >= 11 && skillCardId <= 13)
        return 0; // Attack skills
    if (skillCardId >= 14 && skillCardId <= 16)
        return 1; // Defense skills
    if (skillCardId >= 17 && skillCardId <= 19)
        return 2; // Movement skills

    // Snow White skills
    if (skillCardId >= 23 && skillCardId <= 25)
        return 0; // Attack skills
    if (skillCardId >= 26 && skillCardId <= 28)
        return 1; // Defense skills
    if (skillCardId >= 29 && skillCardId <= 31)
        return 2; // Movement skills

    // Sleeping Beauty skills
    if (skillCardId >= 35 && skillCardId <= 37)
        return 0; // Attack skills

    // Alice skills
    if (skillCardId >= 47 && skillCardId <= 49)
        return 0; // Attack skills
    if (skillCardId >= 50 && skillCardId <= 52)
        return 1; // Defense skills
    if (skillCardId >= 53 && skillCardId <= 55)
        return 2; // Movement skills

    // Pattern for other characters (skills in groups of 3)
    if (skillCardId >= 23 && skillCardId <= 130)
    {
        int skillIndex = (skillCardId - 11) % 9;
        if (skillIndex < 3)
            return 0; // Attack skills
        if (skillIndex < 6)
            return 1; // Defense skills
        return 2;     // Movement skills
    }

    return -1; // Not a skill or invalid
}

int32_t calculate_damage(game *gameState, int32_t cardId, int8_t playerId, int32_t basicCardValue)
{
    (void)gameState; // Suppress unused parameter warning
    (void)playerId;  // Suppress unused parameter warning
    int32_t baseDamage = get_card_value(cardId);

    if (is_skill_card(cardId))
    {
        // For skills, damage = base + basic card value (O)
        baseDamage += basicCardValue;
    }

    // Apply character-specific modifiers
    // Apply metamorphosis effects
    // Apply temporary bonuses

    return baseDamage;
}

int32_t calculate_defense(game *gameState, int32_t cardId, int8_t playerId, int32_t basicCardValue)
{
    (void)gameState; // Suppress unused parameter warning
    (void)playerId;  // Suppress unused parameter warning
    int32_t baseDefense = get_card_value(cardId);

    if (is_skill_card(cardId))
    {
        // For defense skills, defense usually equals basic card value
        baseDefense = basicCardValue;
    }

    return baseDefense;
}

int32_t calculate_movement(game *gameState, int32_t cardId, int8_t playerId, int32_t basicCardValue)
{
    (void)gameState; // Suppress unused parameter warning
    (void)playerId;  // Suppress unused parameter warning
    int32_t baseMovement = get_card_value(cardId);

    if (is_skill_card(cardId))
    {
        // Movement effects vary by skill
        baseMovement = basicCardValue;
    }

    return baseMovement;
}

int32_t get_card_range(int32_t cardId)
{
    // Basic cards
    if (cardId >= 1 && cardId <= 10)
    {
        return basic_cards[cardId - 1].range;
    }

    // Character-specific cards
    if ((cardId >= 11 && cardId <= 22) || (cardId >= 135 && cardId <= 138))
    {
        for (size_t i = 0; i < sizeof(red_riding_hood_cards) / sizeof(CardData); i++)
        {
            if (red_riding_hood_cards[i].id == cardId)
            {
                return red_riding_hood_cards[i].range;
            }
        }
    }

    // Snow White cards
    if ((cardId >= 23 && cardId <= 34) || (cardId >= 131 && cardId <= 134) || (cardId >= 139 && cardId <= 142))
    {
        for (size_t i = 0; i < sizeof(snow_white_cards) / sizeof(CardData); i++)
        {
            if (snow_white_cards[i].id == cardId)
            {
                return snow_white_cards[i].range;
            }
        }
    }

    // Sleeping Beauty cards
    if ((cardId >= 35 && cardId <= 46) || (cardId >= 143 && cardId <= 146))
    {
        for (size_t i = 0; i < sizeof(sleeping_beauty_cards) / sizeof(CardData); i++)
        {
            if (sleeping_beauty_cards[i].id == cardId)
            {
                return sleeping_beauty_cards[i].range;
            }
        }
    }

    // Alice cards
    if ((cardId >= 47 && cardId <= 58) || (cardId >= 147 && cardId <= 150))
    {
        for (size_t i = 0; i < sizeof(alice_cards) / sizeof(CardData); i++)
        {
            if (alice_cards[i].id == cardId)
            {
                return alice_cards[i].range;
            }
        }
    }

    return 0; // Default range
}

const char *get_card_name(int32_t cardId)
{
    // Basic cards
    if (cardId >= 1 && cardId <= 10)
    {
        return basic_cards[cardId - 1].name;
    }

    // Character-specific cards
    if ((cardId >= 11 && cardId <= 22) || (cardId >= 135 && cardId <= 138))
    {
        for (size_t i = 0; i < sizeof(red_riding_hood_cards) / sizeof(CardData); i++)
        {
            if (red_riding_hood_cards[i].id == cardId)
            {
                return red_riding_hood_cards[i].name;
            }
        }
    }

    // Snow White cards
    if ((cardId >= 23 && cardId <= 34) || (cardId >= 131 && cardId <= 134) || (cardId >= 139 && cardId <= 142))
    {
        for (size_t i = 0; i < sizeof(snow_white_cards) / sizeof(CardData); i++)
        {
            if (snow_white_cards[i].id == cardId)
            {
                return snow_white_cards[i].name;
            }
        }
    }

    // Sleeping Beauty cards
    if ((cardId >= 35 && cardId <= 46) || (cardId >= 143 && cardId <= 146))
    {
        for (size_t i = 0; i < sizeof(sleeping_beauty_cards) / sizeof(CardData); i++)
        {
            if (sleeping_beauty_cards[i].id == cardId)
            {
                return sleeping_beauty_cards[i].name;
            }
        }
    }

    // Alice cards
    if ((cardId >= 47 && cardId <= 58) || (cardId >= 147 && cardId <= 150))
    {
        for (size_t i = 0; i < sizeof(alice_cards) / sizeof(CardData); i++)
        {
            if (alice_cards[i].id == cardId)
            {
                return alice_cards[i].name;
            }
        }
    }

    return "Unknown Card";
}

// Helper functions for card effects
bool execute_basic_card_effect(game *gameState, int32_t cardId, int8_t playerId)
{
    player *p = &gameState->players[playerId];
    int32_t value = get_card_value(cardId);
    int32_t type = get_card_type(cardId);

    switch (type)
    {
    case 0: // Attack
        gameState->nowATK += value;
        p->energy += value;
        break;
    case 1: // Defense
        p->defense += value;
        if (p->defense > p->maxdefense)
            p->defense = p->maxdefense;
        p->energy += value;
        break;
    case 2: // Movement
        gameState->nowMOV += value;
        p->energy += value;
        break;
    case 3: // Universal
        // Universal cards can be used as any basic type
        break;
    }

    return true;
}

bool execute_skill_card_effect(game *gameState, int32_t cardId, int8_t playerId, int32_t basicCardValue)
{
    player *p = &gameState->players[playerId];

    // Deduct energy cost
    int32_t cost = get_card_cost(cardId);
    p->energy -= cost;

    // Execute specific skill effects based on card ID
    switch (cardId)
    {
    case 11: // Quick Shot
        return execute_quick_shot(gameState, playerId, basicCardValue);
    case 12: // Precision Shot
        return execute_precision_shot(gameState, playerId, basicCardValue);
    case 13: // Lethal Shot
        return execute_lethal_shot(gameState, playerId, basicCardValue);
    case 14: // Energy Shield
        return execute_energy_shield(gameState, playerId, basicCardValue);
    case 15: // Current Shield
        return execute_current_shield(gameState, playerId, basicCardValue);
    case 16: // Ultimate Shield
        return execute_ultimate_shield(gameState, playerId, basicCardValue);
    case 17: // Ballistic Jet
        return execute_ballistic_jet(gameState, playerId, basicCardValue);
    case 18: // Power Jet
        return execute_power_jet(gameState, playerId, basicCardValue);
    case 19: // Rage Jet
        return execute_rage_jet(gameState, playerId, basicCardValue);
    case 23: // Poison Blade
        return execute_poison_blade(gameState, playerId, basicCardValue);
    case 24: // Toxic Slash
        return execute_toxic_slash(gameState, playerId, basicCardValue);
    case 25: // Venom Strike
        return execute_venom_strike(gameState, playerId, basicCardValue);
    case 26: // Mirror Shield
        return execute_mirror_shield(gameState, playerId, basicCardValue);
    case 27: // Crystal Shield
        return execute_crystal_shield(gameState, playerId, basicCardValue);
    case 28: // Prism Shield
        return execute_prism_shield(gameState, playerId, basicCardValue);
    case 29: // Shadow Step
        return execute_shadow_step(gameState, playerId, basicCardValue);
    case 30: // Phase Step
        return execute_phase_step(gameState, playerId, basicCardValue);
    case 31: // Specter Step
        return execute_specter_step(gameState, playerId, basicCardValue);
    case 35: // Dream Slash
        return execute_dream_slash(gameState, playerId, basicCardValue);
    case 36: // Nightmare Curse
        return execute_nightmare_curse(gameState, playerId, basicCardValue);
    case 37: // Thorny Rose
        return execute_thorny_rose(gameState, playerId, basicCardValue);
    case 47: // Open Game
        return execute_open_game(gameState, playerId);
    case 48: // Turn Game
        return execute_turn_game(gameState, playerId);
    case 50: // Magic Trick
        return execute_magic_trick(gameState, playerId);
    case 53: // Strange Agility
        return execute_strange_agility(gameState, playerId);
    case 56: // Endless Party
        return execute_endless_party(gameState, playerId);
    case 57: // Wonderful Wonder Day
        return execute_wonderful_wonder_day(gameState, playerId);
    default:
        CARD_LOG(LOG_WARNING, "Unimplemented skill card: %d", cardId);
        break;
    }

    return true;
}

bool execute_ultimate_card_effect(game *gameState, int32_t cardId, int8_t playerId)
{
    // Execute ultimate card effects
    switch (cardId)
    {
    case 20: // Wolf's Devour
        return execute_wolfs_devour(gameState, playerId);
    case 21: // System Invasion
        return execute_system_invasion(gameState, playerId);
    case 22: // Revenge Rain
        return execute_revenge_rain(gameState, playerId);
    case 32: // Queen's Decree
        return execute_queen_decree(gameState, playerId);
    case 33: // Poison Mist
        return execute_poison_mist(gameState, playerId);
    case 34: // Eternal Slumber
        return execute_eternal_slumber(gameState, playerId);
    case 44: // Dream Realm
        return execute_dream_realm(gameState, playerId);
    case 45: // Rose Garden
        return execute_rose_garden(gameState, playerId);
    case 46: // Eternal Dream
        return execute_eternal_dream(gameState, playerId);
    default:
        CARD_LOG(LOG_WARNING, "Unimplemented ultimate card: %d", cardId);
        break;
    }

    return true;
}

// Red Riding Hood skill implementations
bool execute_quick_shot(game *gameState, int8_t playerId, int32_t basicCardValue)
{
    if (!gameState || playerId < 0 || playerId >= 4)
    {
        ERROR_LOG("Invalid parameters for Quick Shot");
        return false;
    }

    player *attacker = &gameState->players[playerId];
    int32_t damage = calculate_damage(gameState, 1, basicCardValue, playerId);
    int32_t range = 1;

    // Find target in range
    for (int i = 0; i < (gameState->playerMode == 0 ? 2 : 4); i++)
    {
        if (i == playerId)
            continue;

        int32_t distance = abs(attacker->locate[0] - gameState->players[i].locate[0]);
        if (attacker->locate[1] != gameState->players[i].locate[1])
            continue;
        if (distance <= range && gameState->players[i].life > 0)
        {
            apply_damage(gameState, damage, i, playerId);
            break;
        }
    }

    CARD_LOG(LOG_DEBUG, "Quick Shot: %d damage at range 1", damage);
    return true;
}

bool execute_precision_shot(game *gameState, int8_t playerId, int32_t basicCardValue)
{
    if (!gameState || playerId < 0 || playerId >= 4)
    {
        ERROR_LOG("Invalid parameters for Precision Shot");
        return false;
    }

    player *attacker = &gameState->players[playerId];
    int32_t damage = calculate_damage(gameState, 2, basicCardValue, playerId);
    int32_t range = 2;

    // Find target in range
    for (int i = 0; i < (gameState->playerMode == 0 ? 2 : 4); i++)
    {
        if (i == playerId)
            continue;

        int32_t distance = abs(attacker->locate[0] - gameState->players[i].locate[0]);
        if (attacker->locate[1] != gameState->players[i].locate[1])
            continue;
        if (distance <= range && gameState->players[i].life > 0)
        {
            apply_damage(gameState, damage, i, playerId);
            break;
        }
    }

    CARD_LOG(LOG_DEBUG, "Precision Shot: %d damage at range 2", damage);
    return true;
}

bool execute_lethal_shot(game *gameState, int8_t playerId, int32_t basicCardValue)
{
    if (!gameState || playerId < 0 || playerId >= 4)
    {
        ERROR_LOG("Invalid parameters for Lethal Shot");
        return false;
    }

    player *attacker = &gameState->players[playerId];
    int32_t damage = calculate_damage(gameState, 3, basicCardValue, playerId);
    int32_t range = 3;

    // Find target in range
    for (int i = 0; i < (gameState->playerMode == 0 ? 2 : 4); i++)
    {
        if (i == playerId)
            continue;

        int32_t distance = abs(attacker->locate[0] - gameState->players[i].locate[0]);
        if (attacker->locate[1] != gameState->players[i].locate[1])
            continue;
        if (distance <= range && gameState->players[i].life > 0)
        {
            apply_damage(gameState, damage, i, playerId);
            break;
        }
    }

    CARD_LOG(LOG_DEBUG, "Lethal Shot: %d damage at range 3", damage);
    return true;
}

bool execute_ballistic_jet(game *gameState, int8_t playerId, int32_t basicCardValue)
{
    if (!gameState || playerId < 0 || playerId >= 4)
    {
        ERROR_LOG("Invalid parameters for Ballistic Jet");
        return false;
    }

    player *attacker = &gameState->players[playerId];
    int32_t damage = 1; // Fixed damage
    int32_t knockback = basicCardValue;

    // Find target in range 1
    for (int i = 0; i < (gameState->playerMode == 0 ? 2 : 4); i++)
    {
        if (i == playerId)
            continue;

        int32_t distance = abs(attacker->locate[0] - gameState->players[i].locate[0]);
        if (attacker->locate[1] != gameState->players[i].locate[1])
            continue;
        if (distance <= 1 && gameState->players[i].life > 0)
        {
            // Apply damage
            apply_damage(gameState, damage, i, playerId);

            // Apply knockback
            player *target = &gameState->players[i];
            int32_t direction = target->locate[0] > attacker->locate[0] ? 1 : -1;
            for (int k = 0; k < knockback; k++)
            {
                int32_t newPos = target->locate[0] + direction;
                if (newPos >= 1 && newPos <= 9 && !is_position_occupied(gameState, newPos, i))
                {
                    target->locate[0] = newPos;
                }
                else
                {
                    break;
                }
            }
            break;
        }
    }

    CARD_LOG(LOG_DEBUG, "Ballistic Jet: 1 damage, %d knockback", knockback);
    return true;
}

bool execute_power_jet(game *gameState, int8_t playerId, int32_t basicCardValue)
{
    if (!gameState || playerId < 0 || playerId >= 4)
    {
        ERROR_LOG("Invalid parameters for Power Jet");
        return false;
    }

    player *attacker = &gameState->players[playerId];
    int32_t damage = 2; // Fixed damage
    int32_t knockback = basicCardValue;

    // Find target in range 2
    for (int i = 0; i < (gameState->playerMode == 0 ? 2 : 4); i++)
    {
        if (i == playerId)
            continue;

        int32_t distance = abs(attacker->locate[0] - gameState->players[i].locate[0]);
        if (attacker->locate[1] != gameState->players[i].locate[1])
            continue;
        if (distance <= 2 && gameState->players[i].life > 0)
        {
            // Apply damage
            apply_damage(gameState, damage, i, playerId);

            // Apply knockback
            player *target = &gameState->players[i];
            int32_t direction = target->locate[0] > attacker->locate[0] ? 1 : -1;
            for (int k = 0; k < knockback; k++)
            {
                int32_t newPos = target->locate[0] + direction;
                if (newPos >= 1 && newPos <= 9 && !is_position_occupied(gameState, newPos, i))
                {
                    target->locate[0] = newPos;
                }
                else
                {
                    break;
                }
            }
            break;
        }
    }

    CARD_LOG(LOG_DEBUG, "Power Jet: 2 damage, %d knockback", knockback);
    return true;
}

bool execute_rage_jet(game *gameState, int8_t playerId, int32_t basicCardValue)
{
    if (!gameState || playerId < 0 || playerId >= 4)
    {
        ERROR_LOG("Invalid parameters for Rage Jet");
        return false;
    }

    player *attacker = &gameState->players[playerId];
    int32_t damage = 3; // Fixed damage
    int32_t knockback = basicCardValue;

    // Find target in range 3
    for (int i = 0; i < (gameState->playerMode == 0 ? 2 : 4); i++)
    {
        if (i == playerId)
            continue;

        int32_t distance = abs(attacker->locate[0] - gameState->players[i].locate[0]);
        if (attacker->locate[1] != gameState->players[i].locate[1])
            continue;
        if (distance <= 3 && gameState->players[i].life > 0)
        {
            // Apply damage
            apply_damage(gameState, damage, i, playerId);

            // Apply knockback
            player *target = &gameState->players[i];
            int32_t direction = target->locate[0] > attacker->locate[0] ? 1 : -1;
            for (int k = 0; k < knockback; k++)
            {
                int32_t newPos = target->locate[0] + direction;
                if (newPos >= 1 && newPos <= 9 && !is_position_occupied(gameState, newPos, i))
                {
                    target->locate[0] = newPos;
                }
                else
                {
                    break;
                }
            }
            break;
        }
    }

    CARD_LOG(LOG_DEBUG, "Rage Jet: 3 damage, %d knockback", knockback);
    return true;
}

// Snow White skill implementations
bool execute_poison_blade(game *gameState, int8_t playerId, int32_t basicCardValue)
{
    if (!gameState || playerId < 0 || playerId >= 4)
    {
        ERROR_LOG("Invalid parameters for Poison Blade");
        return false;
    }

    player *attacker = &gameState->players[playerId];
    int32_t damage = calculate_damage(gameState, 1, basicCardValue, playerId);
    int32_t range = 1;
    int32_t poisonCount = 1;

    // Find target in range
    for (int i = 0; i < (gameState->playerMode == 0 ? 2 : 4); i++)
    {
        if (i == playerId)
            continue;

        int32_t distance = abs(attacker->locate[0] - gameState->players[i].locate[0]);
        if (attacker->locate[1] != gameState->players[i].locate[1])
            continue;
        if (distance <= range && gameState->players[i].life > 0)
        {
            // Apply damage
            apply_damage(gameState, damage, i, playerId);
            // Add poison cards
            snow_white_add_poison_to_deck(&gameState->players[i], 1, poisonCount);
            break;
        }
    }

    CARD_LOG(LOG_DEBUG, "Poison Blade: %d damage, 1 level 1 poison", damage);
    return true;
}

bool execute_toxic_slash(game *gameState, int8_t playerId, int32_t basicCardValue)
{
    if (!gameState || playerId < 0 || playerId >= 4)
    {
        ERROR_LOG("Invalid parameters for Toxic Slash");
        return false;
    }

    player *attacker = &gameState->players[playerId];
    int32_t damage = calculate_damage(gameState, 2, basicCardValue, playerId);
    int32_t range = 2;
    int32_t poisonCount = 2;

    // Find target in range
    for (int i = 0; i < (gameState->playerMode == 0 ? 2 : 4); i++)
    {
        if (i == playerId)
            continue;

        int32_t distance = abs(attacker->locate[0] - gameState->players[i].locate[0]);
        if (attacker->locate[1] != gameState->players[i].locate[1])
            continue;
        if (distance <= range && gameState->players[i].life > 0)
        {
            // Apply damage
            apply_damage(gameState, damage, i, playerId);
            // Add poison cards
            snow_white_add_poison_to_deck(&gameState->players[i], 2, poisonCount);
            break;
        }
    }

    CARD_LOG(LOG_DEBUG, "Toxic Slash: %d damage, 2 level 2 poison", damage);
    return true;
}

bool execute_venom_strike(game *gameState, int8_t playerId, int32_t basicCardValue)
{
    if (!gameState || playerId < 0 || playerId >= 4)
    {
        ERROR_LOG("Invalid parameters for Venom Strike");
        return false;
    }

    player *attacker = &gameState->players[playerId];
    int32_t damage = calculate_damage(gameState, 3, basicCardValue, playerId);
    int32_t range = 3;
    int32_t poisonCount = 3;

    // Find target in range
    for (int i = 0; i < (gameState->playerMode == 0 ? 2 : 4); i++)
    {
        if (i == playerId)
            continue;

        int32_t distance = abs(attacker->locate[0] - gameState->players[i].locate[0]);
        if (attacker->locate[1] != gameState->players[i].locate[1])
            continue;
        if (distance <= range && gameState->players[i].life > 0)
        {
            // Apply damage
            apply_damage(gameState, damage, i, playerId);
            // Add poison cards
            snow_white_add_poison_to_deck(&gameState->players[i], 3, poisonCount);
            break;
        }
    }

    CARD_LOG(LOG_DEBUG, "Venom Strike: %d damage, 3 level 3 poison", damage);
    return true;
}

bool snow_white_add_poison_to_deck(player *target, int32_t poisonLevel, int32_t count)
{
    if (!target || poisonLevel < 1 || poisonLevel > 3 || count <= 0)
    {
        ERROR_LOG("Invalid parameters for adding poison");
        return false;
    }

    // Add poison cards to reminder (they'll be shuffled in during cleanup)
    for (int i = 0; i < count; i++)
    {
        pushbackVector(&target->snowWhite.remindPosion, 131 + (poisonLevel - 1));
    }

    return true;
}

bool snow_white_shuffle_poison_to_deck(player *target)
{
    if (!target)
    {
        ERROR_LOG("Invalid target for poison shuffle");
        return false;
    }

    // Move poison cards from reminder to deck
    while (target->snowWhite.remindPosion.SIZE > 0)
    {
        int32_t poisonCard = target->snowWhite.remindPosion.array[0];
        eraseVector(&target->snowWhite.remindPosion, 0);
        pushbackVector(&target->deck, poisonCard);
    }

    // Shuffle deck
    shuffle_vector(&target->deck);

    return true;
}

bool execute_poison_effect(game *gameState, int8_t playerId, int32_t poisonLevel)
{
    if (!gameState || playerId < 0 || playerId >= 4 || poisonLevel < 1 || poisonLevel > 3)
    {
        ERROR_LOG("Invalid parameters for poison effect");
        return false;
    }

    player *target = &gameState->players[playerId];
    int32_t damage = poisonLevel;

    // Apply poison damage directly to life
    target->life -= damage;
    if (target->life < 0)
        target->life = 0;

    CARD_LOG(LOG_DEBUG, "Poison effect: %d damage to player %d", damage, playerId);
    return true;
}

// Red Riding Hood ultimate implementations
bool execute_wolfs_devour(game *gameState, int8_t playerId)
{
    if (!gameState || playerId < 0 || playerId >= 4)
    {
        ERROR_LOG("Invalid parameters for Wolf's Devour");
        return false;
    }

    player *p = &gameState->players[playerId];

    // Add top 2 cards from any skill supply deck to hand
    for (int i = 0; i < 2; i++)
    {
        // TODO: Implement skill supply deck drawing
        // For now, just log the effect
        CARD_LOG(LOG_DEBUG, "Wolf's Devour: Would add skill card to hand");
    }

    return true;
}

bool execute_system_invasion(game *gameState, int8_t playerId)
{
    if (!gameState || playerId < 0 || playerId >= 4)
    {
        ERROR_LOG("Invalid parameters for System Invasion");
        return false;
    }

    // Repeat last action or skill used this turn
    // This needs access to action history, which isn't implemented yet
    CARD_LOG(LOG_DEBUG, "System Invasion: Would repeat last action");
    return true;
}

bool execute_revenge_rain(game *gameState, int8_t playerId)
{
    if (!gameState || playerId < 0 || playerId >= 4)
    {
        ERROR_LOG("Invalid parameters for Revenge Rain");
        return false;
    }

    player *attacker = &gameState->players[playerId];
    int32_t damage = 3;
    int32_t knockback = 3;

    // Apply effects to all enemies in range 3
    for (int i = 0; i < (gameState->playerMode == 0 ? 2 : 4); i++)
    {
        if (i == playerId)
            continue;

        int32_t distance = abs(attacker->locate[0] - gameState->players[i].locate[0]);
        if (attacker->locate[1] != gameState->players[i].locate[1])
            continue;
        if (distance <= 3 && gameState->players[i].life > 0)
        {
            // Apply damage
            apply_damage(gameState, damage, i, playerId);

            // Apply knockback
            player *target = &gameState->players[i];
            int32_t direction = target->locate[0] > attacker->locate[0] ? 1 : -1;
            for (int k = 0; k < knockback; k++)
            {
                int32_t newPos = target->locate[0] + direction;
                if (newPos >= 1 && newPos <= 9 && !is_position_occupied(gameState, newPos, i))
                {
                    target->locate[0] = newPos;
                }
                else
                {
                    break;
                }
            }

            // Force target to discard 3 cards
            for (int j = 0; j < 3 && target->hand.SIZE > 0; j++)
            {
                int32_t cardId = target->hand.array[target->hand.SIZE - 1];
                popbackVector(&target->hand);
                pushbackVector(&target->graveyard, cardId);
            }
        }
    }

    CARD_LOG(LOG_DEBUG, "Revenge Rain: 3 damage, 3 knockback, discard 3");
    return true;
}

// Snow White ultimate implementations
bool execute_poison_mist(game *gameState, int8_t playerId)
{
    if (!gameState || playerId < 0 || playerId >= 4)
    {
        ERROR_LOG("Invalid parameters for Poison Mist");
        return false;
    }

    player *attacker = &gameState->players[playerId];
    int32_t range = 2; // Area effect in range 2

    // Apply to all enemies in range
    for (int i = 0; i < (gameState->playerMode == 0 ? 2 : 4); i++)
    {
        if (i == playerId)
            continue;

        int32_t distance = abs(attacker->locate[0] - gameState->players[i].locate[0]);
        if (attacker->locate[1] != gameState->players[i].locate[1])
            continue;
        if (distance <= range && gameState->players[i].life > 0)
        {
            // Add poison cards of all levels
            snow_white_add_poison_to_deck(&gameState->players[i], 1, 1);
            snow_white_add_poison_to_deck(&gameState->players[i], 2, 1);
            snow_white_add_poison_to_deck(&gameState->players[i], 3, 1);
        }
    }

    CARD_LOG(LOG_DEBUG, "Poison Mist: Added all levels of poison to enemies in range 2");
    return true;
}

bool execute_queen_decree(game *gameState, int8_t playerId)
{
    if (!gameState || playerId < 0 || playerId >= 4)
    {
        ERROR_LOG("Invalid parameters for Queen's Decree");
        return false;
    }

    player *attacker = &gameState->players[playerId];

    // Apply to all enemies regardless of range
    for (int i = 0; i < (gameState->playerMode == 0 ? 2 : 4); i++)
    {
        if (i == playerId)
            continue;
        if (gameState->players[i].life > 0)
        {
            // Shuffle all poison cards from reminder into target's deck
            snow_white_shuffle_poison_to_deck(&gameState->players[i]);
            // Force them to draw 3 cards
            draw_cards(&gameState->players[i], 3);
        }
    }

    CARD_LOG(LOG_DEBUG, "Queen's Decree: Shuffled poison and forced draws");
    return true;
}

bool execute_eternal_slumber(game *gameState, int8_t playerId)
{
    if (!gameState || playerId < 0 || playerId >= 4)
    {
        ERROR_LOG("Invalid parameters for Eternal Slumber");
        return false;
    }

    player *attacker = &gameState->players[playerId];
    int32_t damage = 5; // High base damage
    int32_t range = 3;

    // Find target in range
    for (int i = 0; i < (gameState->playerMode == 0 ? 2 : 4); i++)
    {
        if (i == playerId)
            continue;

        int32_t distance = abs(attacker->locate[0] - gameState->players[i].locate[0]);
        if (attacker->locate[1] != gameState->players[i].locate[1])
            continue;
        if (distance <= range && gameState->players[i].life > 0)
        {
            // Apply damage
            apply_damage(gameState, damage, i, playerId);
            // Add multiple high-level poison cards
            snow_white_add_poison_to_deck(&gameState->players[i], 3, 3);
            break;
        }
    }

    CARD_LOG(LOG_DEBUG, "Eternal Slumber: %d damage + 3 level 3 poison", damage);
    return true;
}

// Sleeping Beauty skill implementations
bool execute_dream_slash(game *gameState, int8_t playerId, int32_t basicCardValue)
{
    if (!gameState || playerId < 0 || playerId >= 4)
    {
        ERROR_LOG("Invalid parameters for Dream Slash");
        return false;
    }

    player *attacker = &gameState->players[playerId];
    int32_t damage = calculate_damage(gameState, 1, basicCardValue, playerId);
    int32_t range = 1;

    // Bonus damage if awakened
    if (attacker->sleepingBeauty.AWAKEN)
    {
        damage += attacker->sleepingBeauty.atkRise;
    }

    // Find target in range
    for (int i = 0; i < (gameState->playerMode == 0 ? 2 : 4); i++)
    {
        if (i == playerId)
            continue;

        int32_t distance = abs(attacker->locate[0] - gameState->players[i].locate[0]);
        if (attacker->locate[1] != gameState->players[i].locate[1])
            continue;
        if (distance <= range && gameState->players[i].life > 0)
        {
            apply_damage(gameState, damage, i, playerId);
            break;
        }
    }

    CARD_LOG(LOG_DEBUG, "Dream Slash: %d damage (Awakened: %d)",
             damage, attacker->sleepingBeauty.AWAKEN);
    return true;
}

bool execute_nightmare_curse(game *gameState, int8_t playerId, int32_t basicCardValue)
{
    if (!gameState || playerId < 0 || playerId >= 4)
    {
        ERROR_LOG("Invalid parameters for Nightmare Curse");
        return false;
    }

    player *attacker = &gameState->players[playerId];
    int32_t damage = calculate_damage(gameState, 2, basicCardValue, playerId);
    int32_t range = 2;

    // Bonus damage and effects if awakened
    if (attacker->sleepingBeauty.AWAKEN)
    {
        damage += attacker->sleepingBeauty.atkRise;
        // Target must discard a card if awakened
        attacker->sleepingBeauty.dayNightmareDrawRemind = 1;
    }

    // Find target in range
    for (int i = 0; i < (gameState->playerMode == 0 ? 2 : 4); i++)
    {
        if (i == playerId)
            continue;

        int32_t distance = abs(attacker->locate[0] - gameState->players[i].locate[0]);
        if (attacker->locate[1] != gameState->players[i].locate[1])
            continue;
        if (distance <= range && gameState->players[i].life > 0)
        {
            apply_damage(gameState, damage, i, playerId);

            // Force discard if awakened
            if (attacker->sleepingBeauty.AWAKEN && gameState->players[i].hand.SIZE > 0)
            {
                int32_t cardId = gameState->players[i].hand.array[gameState->players[i].hand.SIZE - 1];
                popbackVector(&gameState->players[i].hand);
                pushbackVector(&gameState->players[i].graveyard, cardId);
            }
            break;
        }
    }

    CARD_LOG(LOG_DEBUG, "Nightmare Curse: %d damage %s",
             damage, attacker->sleepingBeauty.AWAKEN ? "+ discard effect" : "");
    return true;
}

bool execute_thorny_rose(game *gameState, int8_t playerId, int32_t basicCardValue)
{
    if (!gameState || playerId < 0 || playerId >= 4)
    {
        ERROR_LOG("Invalid parameters for Thorny Rose");
        return false;
    }

    player *attacker = &gameState->players[playerId];
    int32_t damage = calculate_damage(gameState, 3, basicCardValue, playerId);
    int32_t range = 3;

    // Bonus damage and effects if awakened
    if (attacker->sleepingBeauty.AWAKEN)
    {
        damage += attacker->sleepingBeauty.atkRise;
        // Increase attack rise for next turn
        attacker->sleepingBeauty.atkRise++;
        attacker->sleepingBeauty.atkRiseTime = 1;
    }

    // Find target in range
    for (int i = 0; i < (gameState->playerMode == 0 ? 2 : 4); i++)
    {
        if (i == playerId)
            continue;

        int32_t distance = abs(attacker->locate[0] - gameState->players[i].locate[0]);
        if (attacker->locate[1] != gameState->players[i].locate[1])
            continue;
        if (distance <= range && gameState->players[i].life > 0)
        {
            apply_damage(gameState, damage, i, playerId);
            break;
        }
    }

    CARD_LOG(LOG_DEBUG, "Thorny Rose: %d damage (Attack rise next turn: %d)",
             damage, attacker->sleepingBeauty.atkRise);
    return true;
}

// Alice attack skills
bool execute_open_game(game *gameState, int8_t playerId)
{
    if (!gameState || playerId < 0 || playerId >= 4)
    {
        ERROR_LOG("Invalid parameters for Open Game");
        return false;
    }

    player *attacker = &gameState->players[playerId];
    int32_t damage = 1;

    // Add identity bonus
    int32_t atkBonus = 0, defBonus = 0, moveBonus = 0;
    if (alice_get_identity_bonus(attacker, &atkBonus, &defBonus, &moveBonus))
    {
        damage += atkBonus;
    }

    // Find target in range 1
    for (int i = 0; i < (gameState->playerMode == 0 ? 2 : 4); i++)
    {
        if (i == playerId)
            continue;

        int32_t distance = abs(attacker->locate[0] - gameState->players[i].locate[0]);
        if (attacker->locate[1] != gameState->players[i].locate[1])
            continue;
        if (distance <= 1 && gameState->players[i].life > 0)
        {
            // Apply damage
            gameState->totalDamage = damage;
            apply_damage(gameState, damage, i, playerId);
            break;
        }
    }

    // Option to change to Queen of Hearts
    gameState->status = CHANGE_IDENTITY;
    return true;
}

bool execute_turn_game(game *gameState, int8_t playerId)
{
    if (!gameState || playerId < 0 || playerId >= 4)
    {
        ERROR_LOG("Invalid parameters for Turn Game");
        return false;
    }

    player *attacker = &gameState->players[playerId];

    // Find target in range 2
    for (int i = 0; i < (gameState->playerMode == 0 ? 2 : 4); i++)
    {
        if (i == playerId)
            continue;

        int32_t distance = abs(attacker->locate[0] - gameState->players[i].locate[0]);
        if (attacker->locate[1] != gameState->players[i].locate[1])
            continue;
        if (distance <= 2 && gameState->players[i].life > 0)
        {
            // Show target's hand cards
            gameState->nowShowingCards = gameState->players[i].hand;
            // Target is identified by their location
            if (attacker->team == 0)
            {
                // For red team, target is in enemy locations 7-9
                gameState->status = CHOOSECARDS;
            }
            else
            {
                // For blue team, target is in enemy locations 1-3
                gameState->status = CHOOSECARDS;
            }
            break;
        }
    }

    // Option to switch to Queen of Hearts
    gameState->status = CHANGE_IDENTITY;
    return true;
}

// Alice defense skill implementations
bool execute_magic_trick(game *gameState, int8_t playerId)
{
    if (!gameState || playerId < 0 || playerId >= 4)
    {
        ERROR_LOG("Invalid parameters for Magic Trick");
        return false;
    }

    player *defender = &gameState->players[playerId];
    int32_t defense = 1;

    // Add identity bonus
    int32_t atkBonus = 0, defBonus = 0, moveBonus = 0;
    if (alice_get_identity_bonus(defender, &atkBonus, &defBonus, &moveBonus))
    {
        defense += defBonus;
    }

    // Add defense value
    defender->defense += defense;
    if (defender->defense > defender->maxdefense)
    {
        defender->defense = defender->maxdefense;
    }

    // Option to switch to Mad Hatter
    gameState->status = CHANGE_IDENTITY;

    CARD_LOG(LOG_DEBUG, "Magic Trick: Added %d defense", defense);
    return true;
}

// Alice movement skill implementations
bool execute_strange_agility(game *gameState, int8_t playerId)
{
    if (!gameState || playerId < 0 || playerId >= 4)
    {
        ERROR_LOG("Invalid parameters for Strange Agility");
        return false;
    }

    player *mover = &gameState->players[playerId];
    int32_t maxMove = 1;

    // Add identity bonus if applicable
    int32_t atkBonus = 0, defBonus = 0, moveBonus = 0;
    if (alice_get_identity_bonus(mover, &atkBonus, &defBonus, &moveBonus))
    {
        maxMove += moveBonus;
    }

    // Let player choose move distance
    // gameState->maxMoveDis = maxMove;
    gameState->status = CHOOSE_MOVE_DIS;

    // Game loop will handle the actual movement and card draw
    return true;
}

// Alice ultimate skills
bool execute_endless_party(game *gameState, int8_t playerId)
{
    if (!gameState || playerId < 0 || playerId >= 4)
    {
        ERROR_LOG("Invalid parameters for Endless Party");
        return false;
    }

    player *attacker = &gameState->players[playerId];

    // Draw 3 cards
    draw_cards(attacker, 3);

    // Can use all three identities' bonuses this turn
    attacker->alice.riseBasic = 1;

    CARD_LOG(LOG_DEBUG, "Endless Party: Drew 3 cards and enabled all identity bonuses");
    return true;
}

bool execute_wonderful_wonder_day(game *gameState, int8_t playerId)
{
    if (!gameState || playerId < 0 || playerId >= 4)
    {
        ERROR_LOG("Invalid parameters for Wonderful Wonder Day");
        return false;
    }

    player *p = &gameState->players[playerId];

    // Allow free identity switch
    gameState->status = CHOOSE_IDENTITY;

    // Grant bonuses for next turn
    p->alice.riseBasic = 1;    // Basic cards +1 value
    p->alice.restartTurn = 1;  // Can restart turn once
    p->alice.havedrestart = 0; // Reset restart flag

    CARD_LOG(LOG_DEBUG, "Wonderful Wonder Day activated: Basic +1, Turn restart available");
    return true;
}

bool process_attack_cards(game *gameState)
{
    if (!gameState)
    {
        ERROR_LOG("Invalid game state pointer");
        return false;
    }

    player *currentPlayer = &gameState->players[gameState->now_turn_player_id];
    int32_t totalAttack = gameState->nowATK;
    int32_t range = 1; // Basic attack range is 1

    // Find valid target in range
    int32_t targetIndex = -1;
    for (int i = 0; i < (gameState->playerMode == 0 ? 2 : 4); i++)
    {
        if (i == gameState->now_turn_player_id)
            continue;

        // Check if target is in range
        int32_t distance = abs(currentPlayer->locate[0] - gameState->players[i].locate[0]);
        if (currentPlayer->locate[1] != gameState->players[i].locate[1])
            continue; // Must be on same battle track
        if (distance <= range && gameState->players[i].life > 0)
        {
            targetIndex = i;
            break;
        }
    }

    if (targetIndex == -1)
    {
        CARD_LOG(LOG_DEBUG, "No valid target in range");
        return true; // Still considered successful
    }

    // Apply damage (target's defense is reduced first)
    player *target = &gameState->players[targetIndex];
    if (target->defense >= totalAttack)
    {
        target->defense -= totalAttack;
        CARD_LOG(LOG_DEBUG, "Attack fully blocked by defense");
    }
    else
    {
        int32_t remainingDamage = totalAttack - target->defense;
        target->defense = 0;
        target->life -= remainingDamage;
        CARD_LOG(LOG_DEBUG, "Dealt %d damage to player %d", remainingDamage, targetIndex);
    }

    // Grant energy equal to attack value used
    currentPlayer->energy += totalAttack;
    if (currentPlayer->energy > 25)
        currentPlayer->energy = 25;

    return true;
}

bool process_defense_cards(game *gameState)
{
    if (!gameState)
    {
        ERROR_LOG("Invalid game state pointer");
        return false;
    }

    player *currentPlayer = &gameState->players[gameState->now_turn_player_id];
    int32_t totalDefense = gameState->nowDEF;

    // Add defense points up to max
    currentPlayer->defense += totalDefense;
    if (currentPlayer->defense > currentPlayer->maxdefense)
    {
        currentPlayer->defense = currentPlayer->maxdefense;
    }

    // Grant energy equal to defense value used
    currentPlayer->energy += totalDefense;
    if (currentPlayer->energy > 25)
        currentPlayer->energy = 25;

    CARD_LOG(LOG_DEBUG, "Added %d defense points, current defense: %d",
             totalDefense, currentPlayer->defense);
    return true;
}

bool process_movement_cards(game *gameState)
{
    if (!gameState)
    {
        ERROR_LOG("Invalid game state pointer");
        return false;
    }

    player *currentPlayer = &gameState->players[gameState->now_turn_player_id];
    int32_t totalMovement = gameState->nowMOV;
    int32_t moveDirection = 1; // 1 for forward, -1 for backward
    int32_t remainingMove = totalMovement;

    while (remainingMove > 0)
    {
        int32_t nextPos = currentPlayer->locate[0] + moveDirection;

        // Check bounds
        if (nextPos < 1 || nextPos > 9)
        {
            moveDirection *= -1; // Change direction
            continue;
        }

        // Check if position is occupied
        bool occupied = false;
        for (int i = 0; i < (gameState->playerMode == 0 ? 2 : 4); i++)
        {
            if (i == gameState->now_turn_player_id)
                continue;
            if (gameState->players[i].locate[0] == nextPos &&
                gameState->players[i].locate[1] == currentPlayer->locate[1] &&
                gameState->players[i].life > 0)
            {
                occupied = true;
                break;
            }
        }

        if (occupied)
        {
            moveDirection *= -1; // Change direction
            continue;
        }

        // Move to next position
        currentPlayer->locate[0] = nextPos;
        remainingMove--;

        CARD_LOG(LOG_DEBUG, "Moved to position %d", nextPos);
    }

    // Grant energy equal to movement value used
    currentPlayer->energy += totalMovement;
    if (currentPlayer->energy > 25)
        currentPlayer->energy = 25;

    return true;
}

bool handle_skill_usage(game *gameState)
{
    if (!gameState)
    {
        ERROR_LOG("Invalid game state pointer");
        return false;
    }

    player *currentPlayer = &gameState->players[gameState->now_turn_player_id];
    int32_t skillCardId = gameState->nowUsingCardID;

    int32_t basicCardValue = 0;

    // Get basic card value based on skill requirement
    int8_t reqType = get_skill_requirement_type(skillCardId);
    switch (reqType)
    {
    case 0: // Attack
        basicCardValue = gameState->nowATK;
        break;
    case 1: // Defense
        basicCardValue = gameState->nowDEF;
        break;
    case 2: // Movement
        basicCardValue = gameState->nowMOV;
        break;
    default:
        ERROR_LOG("Invalid skill requirement type");
        return false;
    }

    // Check energy cost
    int32_t cost = get_card_cost(skillCardId);
    if (currentPlayer->energy < cost)
    {
        ERROR_LOG("Not enough energy for skill");
        return false;
    }

    // Deduct energy cost
    currentPlayer->energy -= cost;

    // Execute skill effect
    if (!execute_card_effect(gameState, skillCardId, gameState->now_turn_player_id, basicCardValue))
    {
        ERROR_LOG("Failed to execute skill effect");
        return false;
    }

    return true;
}

bool handle_card_purchase(game *gameState)
{
    if (!gameState)
    {
        ERROR_LOG("Invalid game state pointer");
        return false;
    }

    player *currentPlayer = &gameState->players[gameState->now_turn_player_id];

    // Show available cards for purchase
    show_purchasable_cards(gameState);

    // Get card type and cost
    int32_t cardId = gameState->nowUsingCardID;
    int32_t cost = get_card_cost(cardId);

    // Check if player has enough energy
    if (currentPlayer->energy < cost)
    {
        ERROR_LOG("Not enough energy to purchase card");
        return false;
    }

    // Verify card is available in supply
    bool cardFound = false;
    int32_t cardType = get_card_type(cardId);
    int32_t cardLevel = (cardId - 1) % 3 + 1; // Calculate level for basic cards

    if (is_basic_card(cardId))
    {
        // Check basic card supply
        if (gameState->basicBuyDeck[cardType][cardLevel - 1].SIZE > 0)
        {
            cardFound = true;
        }
    }
    else if (is_skill_card(cardId))
    {
        // Check character skill supply
        int8_t cardOwner = get_card_owner_character(cardId);
        if (cardOwner == currentPlayer->character)
        {
            cardFound = true;
        }
    }

    if (!cardFound)
    {
        ERROR_LOG("Card not available in supply");
        return false;
    }

    // Deduct energy and add card to discard pile
    currentPlayer->energy -= cost;
    pushbackVector(&currentPlayer->graveyard, cardId);

    // Handle metamorphosis cards
    if (is_metamorphosis_card(cardId))
    {
        pushbackVector(&currentPlayer->metamorphosis, cardId);
    }

    CARD_LOG(LOG_DEBUG, "Player %d purchased card %d for %d energy",
             gameState->now_turn_player_id, cardId, cost);

    return true;
}

void show_purchasable_cards(game *gameState)
{
    if (!gameState)
        return;

    player *currentPlayer = &gameState->players[gameState->now_turn_player_id];

    CARD_LOG(LOG_DEBUG, "Available cards for purchase (energy: %d):", currentPlayer->energy);

    // Show available basic cards
    for (int type = 0; type < 4; type++)
    {
        for (int level = 0; level < 3; level++)
        {
            if (gameState->basicBuyDeck[type][level].SIZE > 0)
            {
                int32_t cardId = level + 1 + (type * 3);
                int32_t cost = get_card_cost(cardId);
                const char *typeStr;
                switch (type)
                {
                case 0:
                    typeStr = "Attack";
                    break;
                case 1:
                    typeStr = "Defense";
                    break;
                case 2:
                    typeStr = "Movement";
                    break;
                default:
                    typeStr = "Universal";
                    break;
                }
                CARD_LOG(LOG_DEBUG, "- Level %d %s Card (ID: %d, Cost: %d)",
                         level + 1, typeStr, cardId, cost);
            }
        }
    }

    // Show available character skills
    int8_t charId = currentPlayer->character;
    // Skill card IDs start at 11 + (character * 9)
    int32_t baseSkillId = 11 + (charId * 9);
    for (int i = 0; i < 9; i++)
    {
        int32_t skillId = baseSkillId + i;
        int32_t cost = get_card_cost(skillId);
        CARD_LOG(LOG_DEBUG, "- Skill Card (ID: %d, Cost: %d)", skillId, cost);
    }
}
