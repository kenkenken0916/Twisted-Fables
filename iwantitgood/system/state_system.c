#include "state_system.h"
#include "../utils/debug_log.h"
#include <ctype.h>

// Helper function to clear input buffer
void clear_input_buffer(void)
{
    int c;
    while ((c = getchar()) != '\n' && c != EOF)
        ;
}

// Helper function to validate integer input
bool validate_int_input(const char *input, int *value)
{
    char *endptr;
    *value = strtol(input, &endptr, 10);
    return *endptr == '\n' || *endptr == '\0';
}

/*
 * CHOOSE_IDENTITY
 * Returns: int8_t - 1:紅心皇后 2:瘋帽子 3:柴郡貓
 */
int8_t handle_choose_identity(game *gameState)
{
    if (!gameState)
        return 0;

    player *currentPlayer = &gameState->players[gameState->now_turn_player_id];
    if (currentPlayer->character != 3)
        return 0; // Only Alice can choose identity

    char input[128];
    int value;

    printf("\nChoose Alice's Identity:\n");
    printf("1: Queen of Hearts (Attack +1)\n");
    printf("2: Mad Hatter (Defense +1)\n");
    printf("3: Cheshire Cat (Movement +1)\n");
    printf("Enter your choice (1-3): ");

    while (1)
    {
        if (!fgets(input, sizeof(input), stdin))
            continue;
        if (!validate_int_input(input, &value))
        {
            printf("Invalid input. Please enter a number between 1 and 3: ");
            continue;
        }
        if (value >= 1 && value <= 3)
        {
            return (int8_t)value;
        }
        printf("Invalid choice. Please enter a number between 1 and 3: ");
    }
}

/*
 * CHOOSE_TENTACLE_LOCATION
 * Returns: int32_t - tentacle location
 */
int32_t handle_choose_tentacle_location(game *gameState)
{
    if (!gameState)
        return -1;

    char input[128];
    int value;

    printf("\nChoose tentacle location (1-9):\n");
    printf("Current tentacle locations: ");
    for (uint32_t i = 0; i < gameState->tentacle_TOKEN_locate.SIZE; i++)
    {
        printf("%d ", gameState->tentacle_TOKEN_locate.array[i]);
    }
    printf("\nEnter location (1-9): ");

    while (1)
    {
        if (!fgets(input, sizeof(input), stdin))
            continue;
        if (!validate_int_input(input, &value))
        {
            printf("Invalid input. Please enter a number between 1 and 9: ");
            continue;
        }
        if (value >= 1 && value <= 9)
        {
            return value;
        }
        printf("Invalid location. Please enter a number between 1 and 9: ");
    }
}

/*
 * CHOOSE_SPECIAL_CARD
 * Returns: int32_t - special card id
 */
int32_t handle_choose_special_card(game *gameState)
{
    if (!gameState)
        return -1;

    player *currentPlayer = &gameState->players[gameState->now_turn_player_id];
    char input[128];
    int value;

    printf("\nChoose a special card to use:\n");
    for (uint32_t i = 0; i < currentPlayer->specialDeck.SIZE; i++)
    {
        printf("%d: Card %d\n", i + 1, currentPlayer->specialDeck.array[i]);
    }
    printf("Enter card index (1-%d) or 0 to cancel: ", currentPlayer->specialDeck.SIZE);

    while (1)
    {
        if (!fgets(input, sizeof(input), stdin))
            continue;
        if (!validate_int_input(input, &value))
        {
            printf("Invalid input. Please enter a valid number: ");
            continue;
        }
        if (value >= 0 && value <= (int)currentPlayer->specialDeck.SIZE)
        {
            return value ? currentPlayer->specialDeck.array[value - 1] : 0;
        }
        printf("Invalid choice. Please enter a number between 0 and %d: ", currentPlayer->specialDeck.SIZE);
    }
}

/*
 * APPEND_DESTINY_TOKEN
 * Returns: int32_t - -1,-2,-3 meaning skill(atk/def/mov), 1~10 meaning basic(atk/def/mov/general)
 */
int32_t handle_append_destiny_token(game *gameState)
{
    if (!gameState)
        return 0;

    char input[128];
    int value;

    printf("\nChoose token type to append:\n");
    printf("Skills: -1:Attack, -2:Defense, -3:Movement\n");
    printf("Basic: 1-10 for Basic cards\n");
    printf("Enter your choice (-3 to 10): ");

    while (1)
    {
        if (!fgets(input, sizeof(input), stdin))
            continue;
        if (!validate_int_input(input, &value))
        {
            printf("Invalid input. Please enter a number between -3 and 10: ");
            continue;
        }
        if (value >= -3 && value <= 10 && value != 0)
        {
            return value;
        }
        printf("Invalid choice. Please enter a number between -3 and 10 (except 0): ");
    }
}

/*
 * SET_TARGET_LOCATE_TO_NEARBY
 * Returns: int32_t - set location(1~9)
 */
int32_t handle_set_target_locate_to_nearby(game *gameState)
{
    if (!gameState)
        return -1;

    player *currentPlayer = &gameState->players[gameState->now_turn_player_id];
    char input[128];
    int value;
    int currentLoc = currentPlayer->locate[0];

    printf("\nChoose a location nearby (1-9):\n");
    printf("Current location: %d\n", currentLoc);
    printf("Enter target location (1-9): ");

    while (1)
    {
        if (!fgets(input, sizeof(input), stdin))
            continue;
        if (!validate_int_input(input, &value))
        {
            printf("Invalid input. Please enter a number between 1 and 9: ");
            continue;
        }
        if (value >= 1 && value <= 9)
        {
            // Validate if location is nearby (difference of 1)
            if (abs(value - currentLoc) <= 1)
            {
                return value;
            }
            printf("Location must be adjacent to current position.\n");
            continue;
        }
        printf("Invalid location. Please enter a number between 1 and 9: ");
    }
}

/*
 * CHOOSE_MOVE
 * Returns: int32_t - choose moves
 * (0:focus, 1-3:use basic cards(1:atk,2:def,3:mov),
 *  4:use a skill card, 5:use a special card, 6:buy a card,
 *  7:metamorphosis, 8:charactor special move,9:drop poison
 *  10:end)
 */
int32_t handle_choose_move(game *gameState, int32_t *returnValue)
{
    if (!gameState)
        return -1;

    char input[128];
    int value;

    printf("\nChoose your move:\n");
    printf("0: Focus\n");
    printf("1-3: Use basic cards (1:Attack, 2:Defense, 3:Movement)\n");
    printf("4: Use a skill card\n");
    printf("5: Use a special card\n");
    printf("6: Buy a card\n");
    printf("7: Metamorphosis\n");
    printf("8: Character special move\n");
    printf("9: Drop poison\n");
    printf("10: End turn\n");
    printf("Enter your choice (0-10): ");

    while (1)
    {
        if (!fgets(input, sizeof(input), stdin))
            continue;
        if (!validate_int_input(input, &value))
        {
            printf("Invalid input. Please enter a number between 0 and 10: ");
            continue;
        }
        if (value >= 0 && value <= 10)
        {

            (*returnValue) = value;
            return 1;
        }
        printf("Invalid choice. Please enter a number between 0 and 10: ");
    }
}

/*
 * BUY_CARD_TYPE
 * Returns: int32_t - -1,-2,-3 meaning skill(atk/def/mov), 1~10 meaning basic(atk/def/mov/general)
 */
int32_t handle_buy_card_type(game *gameState)
{
    if (!gameState)
        return 0;

    char input[128];
    int value;

    printf("\nChoose card type to buy:\n");
    printf("Skills: -1:Attack, -2:Defense, -3:Movement\n");
    printf("Basic: 1-10 for Basic cards\n");
    printf("Enter your choice (-3 to 10): ");

    while (1)
    {
        if (!fgets(input, sizeof(input), stdin))
            continue;
        if (!validate_int_input(input, &value))
        {
            printf("Invalid input. Please enter a number between -3 and 10: ");
            continue;
        }
        if (value >= -3 && value <= 10 && value != 0)
        {
            return value;
        }
        printf("Invalid choice. Please enter a number between -3 and 10 (except 0): ");
    }
}

/*
 * REMOVE_HG
 * Returns: int32_t - for remove card, choose hand or graveyard(negtive:graveyard, positive:hand, value:index of card, **1 base**)
 */
int32_t handle_remove_hg(game *gameState, int32_t *returnValue)
{
    if (!gameState)
        return 0;

    player *currentPlayer = &gameState->players[gameState->now_turn_player_id];
    char input[128];
    int value;

    printf("\nChoose card to remove:\n");
    printf("Hand cards (positive index):\n");
    for (uint32_t i = 0; i < currentPlayer->hand.SIZE; i++)
    {
        printf("%d: Card %d\n", i + 1, currentPlayer->hand.array[i]);
    }
    printf("\nGraveyard cards (negative index):\n");
    for (uint32_t i = 0; i < currentPlayer->graveyard.SIZE; i++)
    {
        printf("-%d: Card %d\n", i + 1, currentPlayer->graveyard.array[i]);
    }
    printf("Enter card index (negative for graveyard, positive for hand): ");

    while (1)
    {
        if (!fgets(input, sizeof(input), stdin))
            continue;
        if (!validate_int_input(input, &value))
        {
            printf("Invalid input. Please enter a valid number: ");
            continue;
        }
        if (value < 0 && (uint32_t)(-value) <= currentPlayer->graveyard.SIZE)
        {
            (*returnValue) = value;
            return 1;
        }
        if (value > 0 && (uint32_t)value <= currentPlayer->hand.SIZE)
        {
            (*returnValue) = value;
            return 1;
        }
        if (value == 0)
        {
            printf("Cannot choose index 0. ");
        }
        printf("Please enter a valid index: ");
    }
}

/*
 * DROP_H
 * Returns: int32_t - for drop card, choose hand(value:index of card, **1 base**)
 */
int32_t handle_drop_h(game *gameState)
{
    if (!gameState)
        return 0;

    player *currentPlayer = &gameState->players[gameState->now_turn_player_id];
    char input[128];
    int value;

    printf("\nChoose card to drop from hand:\n");
    for (uint32_t i = 0; i < currentPlayer->hand.SIZE; i++)
    {
        printf("%d: Card %d\n", i + 1, currentPlayer->hand.array[i]);
    }
    printf("Enter card index (1-%d): ", currentPlayer->hand.SIZE);

    while (1)
    {
        if (!fgets(input, sizeof(input), stdin))
            continue;
        if (!validate_int_input(input, &value))
        {
            printf("Invalid input. Please enter a valid number: ");
            continue;
        }
        if (value >= 1 && (uint32_t)value <= currentPlayer->hand.SIZE)
        {
            return value;
        }
        printf("Invalid choice. Please enter a number between 1 and %d: ", currentPlayer->hand.SIZE);
    }
}

/*
 * USE_ATK
 * Returns: int32_t - for use atk basic card, choose hand(value:index of card, **1 base**, 0 is meaning stop use)
 */
int32_t handle_use_atk(game *gameState, int32_t *returnValue)
{
    if (!gameState || !returnValue)
    {
        return 0;
    }

    player *currentPlayer = &gameState->players[gameState->now_turn_player_id];
    char input[128];
    int value;
    int selectedCount = -1;
    bool validCardsExist = false;

    while (1)
    {
        validCardsExist = false;
        printf("\nChoose attack card to use (or 0 to stop):\n");
        printf("Currently selected cards: ");
        for (int i = 0; i < selectedCount; i++)
        {
            printf("%d ", returnValue[i]);
        }
        printf("\n"); // Show available attack cards
        for (uint32_t i = 0; i < currentPlayer->hand.SIZE; i++)
        {
            int32_t cardId = currentPlayer->hand.array[i];
            int32_t cardType = get_card_type(cardId);
            bool alreadySelected = false;

            // Check if card was already selected
            for (int j = 0; j < selectedCount; j++)
            {
                if (returnValue[j] == (int32_t)(i + 1))
                {
                    alreadySelected = true;
                    break;
                }
            }

            // cardType 0 is ATK (basic attack or attack skill)
            if (cardType == 0 && !alreadySelected){
                printf("%d: Card %d Basic Attack\n", i + 1, cardId);
                validCardsExist = true;
            }
        }

        if (!validCardsExist && selectedCount == 0)
        {
            printf("No attack cards available.\n");
            return 0;
        }
        else if (!validCardsExist)
        {
            printf("No more attack cards available.\n");
            return selectedCount;
        }

        printf("Enter card index (1-%d) or 0 to stop: ", currentPlayer->hand.SIZE);

        if (!fgets(input, sizeof(input), stdin))
        {
            continue;
        }

        if (!validate_int_input(input, &value))
        {
            printf("Invalid input. Please enter a valid number.\n");
            continue;
        }

        if (value == 0)
        {
            return selectedCount;
        }

        if (value < 0 || (uint32_t)value > currentPlayer->hand.SIZE)
        {
            printf("Invalid card index. Please choose between 0 and %d.\n", currentPlayer->hand.SIZE);
            continue;
        }

        // Check if card is attack type
        int32_t selectedCardId = currentPlayer->hand.array[value - 1];
        int32_t selectedCardType = get_card_type(selectedCardId);
        bool isAttackCard = (selectedCardType == 0);

        if (!isAttackCard)
        {
            printf("Selected card must be an attack card (basic attack or attack skill).\n");
            continue;
        }

        // Check if card was already selected
        bool alreadySelected = false;
        for (int i = 0; i < selectedCount; i++)
        {
            if (returnValue[i] == value)
            {
                alreadySelected = true;
                break;
            }
        }

        if (alreadySelected)
        {
            printf("This card has already been selected.\n");
            continue;
        }

        returnValue[selectedCount++] = value;
        printf("Card %d added to selection.\n", value);
    }
}

/*
 * USE_DEF
 * Returns: int32_t - for use def basic card, choose hand(value:index of card, **1 base**, 0 is meaning stop use)
 */
int32_t handle_use_def(game *gameState, int32_t *returnValue)
{
    if (!gameState || !returnValue)
    {
        return 0;
    }

    player *currentPlayer = &gameState->players[gameState->now_turn_player_id];
    char input[128];
    int value;
    int selectedCount = 0;
    bool validCardsExist = false;

    while (1)
    {
        validCardsExist = false;
        printf("\nChoose defense card to use (or 0 to stop):\n");
        printf("Currently selected cards: ");
        for (int i = 0; i < selectedCount; i++)
        {
            printf("%d ", returnValue[i]);
        }
        printf("\n");

        for (uint32_t i = 0; i < currentPlayer->hand.SIZE; i++)
        {
            int32_t cardId = currentPlayer->hand.array[i];
            int32_t cardType = get_card_type(cardId);
            bool alreadySelected = false;

            // Check if card was already selected
            for (int j = 0; j < selectedCount; j++)
            {
                if (returnValue[j] == (int32_t)(i + 1))
                {
                    alreadySelected = true;
                    break;
                }
            }

            // cardType 1 is DEF (basic defense or defense skill)
            if (cardType == 1 && !alreadySelected)
            {
                printf("%d: Card %d Basic Defense\n", i + 1, cardId);
                validCardsExist = true;
            }
        }

        if (!validCardsExist && selectedCount == 0)
        {
            printf("No defense cards available.\n");
            return 0;
        }
        else if (!validCardsExist)
        {
            printf("No more defense cards available.\n");
            return selectedCount;
        }

        printf("Enter card index (1-%d) or 0 to stop: ", currentPlayer->hand.SIZE);

        if (!fgets(input, sizeof(input), stdin))
        {
            continue;
        }

        if (!validate_int_input(input, &value))
        {
            printf("Invalid input. Please enter a valid number.\n");
            continue;
        }

        if (value == 0)
        {
            return selectedCount;
        }

        if (value < 0 || (uint32_t)value > currentPlayer->hand.SIZE)
        {
            printf("Invalid card index. Please choose between 0 and %d.\n", currentPlayer->hand.SIZE);
            continue;
        }

        // Check if card is defense type
        int32_t selectedCardId = currentPlayer->hand.array[value - 1];
        int32_t selectedCardType = get_card_type(selectedCardId);
        bool isDefenseCard = (selectedCardType == 1);

        if (!isDefenseCard)
        {
            printf("Selected card must be a defense card (basic defense or defense skill).\n");
            continue;
        }

        // Check if card was already selected
        bool alreadySelected = false;
        for (int i = 0; i < selectedCount; i++)
        {
            if (returnValue[i] == value)
            {
                alreadySelected = true;
                break;
            }
        }

        if (alreadySelected)
        {
            printf("This card has already been selected.\n");
            continue;
        }

        returnValue[selectedCount++] = value;
        printf("Card %d added to selection.\n", value);
    }
}

/*
 * USE_MOV
 * Returns: int32_t - for use mov basic card, choose hand(value:index of card, **1 base**, 0 is meaning stop use)
 */
int32_t handle_use_mov(game *gameState, int32_t *returnValue)
{
    if (!gameState || !returnValue)
    {
        return 0;
    }

    player *currentPlayer = &gameState->players[gameState->now_turn_player_id];
    char input[128];
    int value;
    int selectedCount = 0;
    bool validCardsExist = false;

    while (1)
    {
        validCardsExist = false;
        printf("\nChoose movement card to use (or 0 to stop):\n");
        printf("Currently selected cards: ");
        for (int i = 0; i < selectedCount; i++)
        {
            printf("%d ", returnValue[i]);
        }
        printf("\n");

        for (uint32_t i = 0; i < currentPlayer->hand.SIZE; i++)
        {
            int32_t cardId = currentPlayer->hand.array[i];
            int32_t cardType = get_card_type(cardId);
            bool alreadySelected = false;

            // Check if card was already selected
            for (int j = 0; j < selectedCount; j++)
            {
                if (returnValue[j] == (int32_t)(i + 1))
                {
                    alreadySelected = true;
                    break;
                }
            }

            // cardType 2 is MOV (basic movement or movement skill)
            if (cardType == 2 && !alreadySelected)
            {
                printf("%d: Card %d Basic Movement\n", i + 1, cardId);
                validCardsExist = true;
            }
        }

        if (!validCardsExist && selectedCount == 0)
        {
            printf("No movement cards available.\n");
            return 0;
        }
        else if (!validCardsExist)
        {
            printf("No more movement cards available.\n");
            return selectedCount;
        }

        printf("Enter card index (1-%d) or 0 to stop: ", currentPlayer->hand.SIZE);

        if (!fgets(input, sizeof(input), stdin))
        {
            continue;
        }

        if (!validate_int_input(input, &value))
        {
            printf("Invalid input. Please enter a valid number.\n");
            continue;
        }

        if (value == 0)
        {
            return selectedCount;
        }

        if (value < 0 || (uint32_t)value > currentPlayer->hand.SIZE)
        {
            printf("Invalid card index. Please choose between 0 and %d.\n", currentPlayer->hand.SIZE);
            continue;
        }

        // Check if card is movement type
        int32_t selectedCardId = currentPlayer->hand.array[value - 1];
        int32_t selectedCardType = get_card_type(selectedCardId);
        bool isMovementCard = (selectedCardType == 2);

        if (!isMovementCard)
        {
            printf("Selected card must be a movement card (basic movement or movement skill).\n");
            continue;
        }

        // Check if card was already selected
        bool alreadySelected = false;
        for (int i = 0; i < selectedCount; i++)
        {
            if (returnValue[i] == value)
            {
                alreadySelected = true;
                break;
            }
        }

        if (alreadySelected)
        {
            printf("This card has already been selected.\n");
            continue;
        }

        returnValue[selectedCount++] = value;
        printf("Card %d added to selection.\n", value);
    }
}
