#include "game_init.h"
#include "../systems/card_system.h"
#include "../systems/character_system.h"
#include "../utils/debug_log.h"
#include "../utils/utils.h"

bool setup_initial_game(game *gameState)
{
    if (!gameState)
    {
        ERROR_LOG("Invalid game state pointer");
        return false;
    }

    GAME_LOG(LOG_DEBUG, "Setting up initial game");

    // Setup basic supply decks
    if (!setup_basic_supply_decks(gameState))
    {
        ERROR_LOG("Failed to setup basic supply decks");
        return false;
    }

    // Setup skill supply decks
    if (!setup_skill_supply_decks(gameState))
    {
        ERROR_LOG("Failed to setup skill supply decks");
        return false;
    }

    // Setup initial positions
    if (!setup_initial_positions(gameState))
    {
        ERROR_LOG("Failed to setup initial positions");
        return false;
    }

    // Setup initial hands
    if (!setup_initial_hands(gameState))
    {
        ERROR_LOG("Failed to setup initial hands");
        return false;
    }

    GAME_LOG(LOG_INFO, "Initial game setup completed");
    return true;
}

bool init_player(player *player, int playerIndex)
{
    if (!player)
    {
        ERROR_LOG("Invalid player pointer");
        return false;
    }

    GAME_LOG(LOG_DEBUG, "Initializing player %d", playerIndex);

    // Initialize basic properties
    player->team = (playerIndex % 2); // Team assignment for 2v2
    player->locate[0] = 5;            // Default center position
    player->locate[1] = 0;            // Battle track 0
    player->character = 0;            // Default to Red Riding Hood
    player->energy = 0;
    player->specialGate = 0;

    // Initialize character stats (will be set when character is chosen)
    player->maxlife = 30;
    player->life = 30;
    player->maxdefense = 6;
    player->defense = 0;

    // Initialize vectors
    player->hand = initVector();
    player->deck = initVector();
    player->usecards = initVector();
    player->graveyard = initVector();
    player->metamorphosis = initVector();
    player->attackSkill = initVector();
    player->defenseSkill = initVector();
    player->moveSkill = initVector();
    player->specialDeck = initVector();

    // Initialize character-specific structures
    init_character_specific_data(player);

    return true;
}

void cleanup_player(player *player)
{
    if (!player)
    {
        WARNING_LOG("Invalid player pointer during cleanup");
        return;
    }

    // Clear all vectors
    clearVector(&player->hand);
    clearVector(&player->deck);
    clearVector(&player->usecards);
    clearVector(&player->graveyard);
    clearVector(&player->metamorphosis);
    clearVector(&player->attackSkill);
    clearVector(&player->defenseSkill);
    clearVector(&player->moveSkill);
    clearVector(&player->specialDeck);

    // Clear character-specific vectors
    clearVector(&player->snowWhite.remindPosion);
    clearVector(&player->scheherazade.destiny_TOKEN_locate);
    clearVector(&player->scheherazade.destiny_TOKEN_type);
}

bool validate_player(player *player)
{
    if (!player)
    {
        ERROR_LOG("Player is NULL");
        return false;
    }

    // Validate basic stats (note: these are uint8_t so no need to check < 0)
    if (player->life > player->maxlife)
    {
        ERROR_LOG("Invalid life: %d/%d", player->life, player->maxlife);
        return false;
    }

    if (player->defense > player->maxdefense)
    {
        ERROR_LOG("Invalid defense: %d/%d", player->defense, player->maxdefense);
        return false;
    }

    if (player->energy > 25)
    {
        ERROR_LOG("Invalid energy: %d", player->energy);
        return false;
    }

    if (player->character > 9)
    {
        ERROR_LOG("Invalid character: %d", player->character);
        return false;
    }

    // Validate position
    if (player->locate[0] < 1 || player->locate[0] > 9)
    {
        ERROR_LOG("Invalid position: %d", player->locate[0]);
        return false;
    }

    if (player->locate[1] > 1)
    {
        ERROR_LOG("Invalid battle track: %d", player->locate[1]);
        return false;
    }

    return true;
}

bool setup_initial_deck(player *player, uint8_t characterId)
{
    if (!player)
    {
        ERROR_LOG("Invalid player pointer");
        return false;
    }

    GAME_LOG(LOG_DEBUG, "Setting up initial deck for character %d", characterId);

    // Set character stats based on character ID
    set_character_stats(player, characterId);

    // Clear existing deck
    clearVector(&player->deck);

    // Add initial deck composition (12 cards total):
    // 3 Level-1 skills, 3 Level-1 attack, 3 Level-1 defense, 3 Level-1 movement

    // Add 3 Level-1 attack cards (ID: 1)
    for (int i = 0; i < 3; i++)
    {
        pushbackVector(&player->deck, 1);
    }

    // Add 3 Level-1 defense cards (ID: 4)
    for (int i = 0; i < 3; i++)
    {
        pushbackVector(&player->deck, 4);
    }

    // Add 3 Level-1 movement cards (ID: 7)
    for (int i = 0; i < 3; i++)
    {
        pushbackVector(&player->deck, 7);
    }

    // Add 3 Level-1 skills based on character
    add_initial_character_skills(player, characterId);

    // Shuffle the deck
    shuffle_vector(&player->deck);

    return true;
}

bool setup_basic_supply_decks(game *gameState)
{
    if (!gameState)
    {
        ERROR_LOG("Invalid game state pointer");
        return false;
    }

    GAME_LOG(LOG_DEBUG, "Setting up basic supply decks");

    // Setup Attack cards (Level 1-3, IDs 1-3)
    for (int level = 0; level < 3; level++)
    {
        clearVector(&gameState->basicBuyDeck[0][level]);
        for (int i = 0; i < 18; i++)
        {
            pushbackVector(&gameState->basicBuyDeck[0][level], level + 1);
        }
    }

    // Setup Defense cards (Level 1-3, IDs 4-6)
    for (int level = 0; level < 3; level++)
    {
        clearVector(&gameState->basicBuyDeck[1][level]);
        for (int i = 0; i < 18; i++)
        {
            pushbackVector(&gameState->basicBuyDeck[1][level], level + 4);
        }
    }

    // Setup Movement cards (Level 1-3, IDs 7-9)
    for (int level = 0; level < 3; level++)
    {
        clearVector(&gameState->basicBuyDeck[2][level]);
        for (int i = 0; i < 18; i++)
        {
            pushbackVector(&gameState->basicBuyDeck[2][level], level + 7);
        }
    }

    // Setup Generic cards (ID: 10)
    for (int level = 0; level < 3; level++)
    {
        clearVector(&gameState->basicBuyDeck[3][level]);
        if (level == 0)
        { // Only Level 1 generic cards
            for (int i = 0; i < 18; i++)
            {
                pushbackVector(&gameState->basicBuyDeck[3][level], 10);
            }
        }
    }

    return true;
}

bool setup_skill_supply_decks(game *gameState)
{
    if (!gameState)
    {
        ERROR_LOG("Invalid game state pointer");
        return false;
    }

    GAME_LOG(LOG_DEBUG, "Setting up skill supply decks");

    // This would setup skill cards for each character
    // For now, we'll just ensure the structure is ready

    // In a full implementation, this would populate skill supply
    // decks with all character skill cards

    return true;
}

bool setup_initial_hands(game *gameState)
{
    if (!gameState)
    {
        ERROR_LOG("Invalid game state pointer");
        return false;
    }

    GAME_LOG(LOG_DEBUG, "Setting up initial hands");

    int playerCount = (gameState->playerMode == 0) ? 2 : 4;

    for (int i = 0; i < playerCount; i++)
    {
        player *p = &gameState->players[i];

        // Clear hand
        clearVector(&p->hand);

        // First player draws 4 cards, others draw 6
        int cardsToDraw = (i == 0) ? 4 : 6;

        for (int j = 0; j < cardsToDraw && p->deck.SIZE > 0; j++)
        {
            int32_t cardId = p->deck.array[p->deck.SIZE - 1];
            popbackVector(&p->deck);
            pushbackVector(&p->hand, cardId);
        }

        GAME_LOG(LOG_DEBUG, "Player %d drew %d cards", i, p->hand.SIZE);
    }

    return true;
}

bool setup_initial_positions(game *gameState)
{
    if (!gameState)
    {
        ERROR_LOG("Invalid game state pointer");
        return false;
    }

    GAME_LOG(LOG_DEBUG, "Setting up initial positions");

    if (gameState->playerMode == 0)
    {
        // 1v1 mode: players at positions 3 and 7
        gameState->players[0].locate[0] = 3;
        gameState->players[0].locate[1] = 0;
        gameState->players[1].locate[0] = 7;
        gameState->players[1].locate[1] = 0;
    }
    else
    {
        // 2v2 mode: teammates cannot be on same track
        gameState->players[0].locate[0] = 3;
        gameState->players[0].locate[1] = 0;
        gameState->players[1].locate[0] = 7;
        gameState->players[1].locate[1] = 0;
        gameState->players[2].locate[0] = 3;
        gameState->players[2].locate[1] = 1;
        gameState->players[3].locate[0] = 7;
        gameState->players[3].locate[1] = 1;
    }

    return true;
}

// Helper functions
void init_character_specific_data(player *player)
{
    // Initialize all character-specific data to default values

    // Red Riding Hood
    for (int i = 0; i < 3; i++)
    {
        player->redHood.saveCard[i] = -1;
    }

    // Snow White
    player->snowWhite.remindPosion = initVector();

    // Sleeping Beauty
    player->sleepingBeauty.AWAKEN_TOKEN = 0;
    player->sleepingBeauty.AWAKEN = 0;
    player->sleepingBeauty.dayNightmareDrawRemind = 0;
    player->sleepingBeauty.atkRise = 0;
    player->sleepingBeauty.atkRiseTime = 0;
    player->sleepingBeauty.usedmeta1 = 0;

    // Alice
    player->alice.identity = 0;
    player->alice.riseBasic = 0;
    player->alice.restartTurn = 0;
    player->alice.havedrestart = 0;

    // Mulan
    player->mulan.KI_TOKEN = 0;
    player->mulan.extraCard = 0;
    player->mulan.extraDraw = 0;

    // Princess Kaguya
    player->kaguya.useDefenseAsATK = 0;
    player->kaguya.useMoveTarget = 0;

    // Match Girl
    player->matchGirl.remindMatch = 0;
    player->matchGirl.pushedMatch = 0;

    // Dorothy
    player->dorothy.COMBO_TOKEN = 0;
    player->dorothy.canCombo = 0;

    // Scheherazade
    player->scheherazade.destiny_TOKEN_locate = initVector();
    player->scheherazade.destiny_TOKEN_type = initVector();
    player->scheherazade.selectToken = 0;
}

void set_character_stats(player *player, uint8_t characterId)
{
    switch (characterId)
    {
    case 0: // Red Riding Hood
        player->maxlife = 30;
        player->life = 30;
        player->maxdefense = 6;
        player->specialGate = 15;
        break;
    case 1: // Snow White
        player->maxlife = 34;
        player->life = 34;
        player->maxdefense = 6;
        player->specialGate = 17;
        break;
    case 2: // Sleeping Beauty
        player->maxlife = 42;
        player->life = 42;
        player->maxdefense = 6;
        player->specialGate = 21;
        break;
    case 3: // Alice
        player->maxlife = 32;
        player->life = 32;
        player->maxdefense = 6;
        player->specialGate = 16;
        break;
    case 4: // Mulan
        player->maxlife = 34;
        player->life = 34;
        player->maxdefense = 3;
        player->specialGate = 17;
        break;
    case 5: // Princess Kaguya
        player->maxlife = 32;
        player->life = 32;
        player->maxdefense = 6;
        player->specialGate = 16;
        break;
    case 6: // Little Mermaid
        player->maxlife = 36;
        player->life = 36;
        player->maxdefense = 3;
        player->specialGate = 18;
        break;
    case 7: // Match Girl
        player->maxlife = 36;
        player->life = 36;
        player->maxdefense = 6;
        player->specialGate = 18;
        break;
    case 8: // Dorothy
        player->maxlife = 40;
        player->life = 40;
        player->maxdefense = 6;
        player->specialGate = 20;
        break;
    case 9: // Scheherazade
        player->maxlife = 36;
        player->life = 36;
        player->maxdefense = 6;
        player->specialGate = 18;
        break;
    default:
        ERROR_LOG("Unknown character ID: %d", characterId);
        break;
    }

    player->character = characterId;
    player->defense = 0;
}

void add_initial_character_skills(player *player, uint8_t characterId)
{
    // Add 3 Level-1 skills based on character
    // For Red Riding Hood (Character 0): Quick Shot (11), Energy Shield (14), Ballistic Jet (17)

    switch (characterId)
    {
    case 0:                                // Red Riding Hood
        pushbackVector(&player->deck, 11); // Quick Shot
        pushbackVector(&player->deck, 14); // Energy Shield
        pushbackVector(&player->deck, 17); // Ballistic Jet
        break;
    case 1:                                // Snow White
        pushbackVector(&player->deck, 23); // Crystal Shard
        pushbackVector(&player->deck, 26); // Corrupted Blessing
        pushbackVector(&player->deck, 29); // Broken Fantasy
        break;
    case 2:                                // Sleeping Beauty
        pushbackVector(&player->deck, 35); // Mind Tremor
        pushbackVector(&player->deck, 38); // Burst Lock
        pushbackVector(&player->deck, 41); // Dark Touch
        break;
    case 3:                                // Alice
        pushbackVector(&player->deck, 47); // Open Game
        pushbackVector(&player->deck, 50); // Magic Trick
        pushbackVector(&player->deck, 53); // Strange Agility
        break;
    case 4:                                // Mulan
        pushbackVector(&player->deck, 59); // Not to be Underestimated
        pushbackVector(&player->deck, 62); // Still Controls Moving
        pushbackVector(&player->deck, 65); // Never Retreat
        break;
    case 5:                                // Princess Kaguya
        pushbackVector(&player->deck, 71); // Light of Understanding
        pushbackVector(&player->deck, 74); // Echo of Confusion
        pushbackVector(&player->deck, 77); // Focus Introspection
        break;
    case 6:                                // Little Mermaid
        pushbackVector(&player->deck, 83); // Siren's Call
        pushbackVector(&player->deck, 86); // Surging Rage
        pushbackVector(&player->deck, 89); // Abyss Consume
        break;
    case 7:                                 // Match Girl
        pushbackVector(&player->deck, 95);  // Illusory Wish
        pushbackVector(&player->deck, 98);  // Devil's Offering
        pushbackVector(&player->deck, 101); // Weightless Soul
        break;
    case 8:                                 // Dorothy
        pushbackVector(&player->deck, 107); // Target Confirm
        pushbackVector(&player->deck, 110); // Mind Probe
        pushbackVector(&player->deck, 113); // Enemy Track
        break;
    case 9:                                 // Scheherazade
        pushbackVector(&player->deck, 119); // Erase Dream
        pushbackVector(&player->deck, 122); // Submerge Net
        pushbackVector(&player->deck, 125); // Watch Eye
        break;
    default:
        ERROR_LOG("Unknown character ID for skill setup: %d", characterId);
        break;
    }
}
