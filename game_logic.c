#include <stdio.h>
#include <stdlib.h>
#include "game_logic.h"
#include "debug_log.h"
#include "card_combination.h"
#include "architecture.h"
#include "character_system.h"

void display_game_state(game *gameState)
{
    INFO_LOG("Display Game State - Current Player: %d", gameState->now_turn_player_id + 1);

    printf("\n=== Game State ===\n");
    printf("Current Player: %d\n", gameState->now_turn_player_id + 1);

    // Display player information
    player *current_player = &gameState->players[gameState->now_turn_player_id];
    printf("HP: %d/%d\n", current_player->life, current_player->maxlife);
    printf("Defense: %d/%d\n", current_player->defense, current_player->maxdefense);
    printf("Energy: %d\n", current_player->energy);
    printf("Position: %d,%d\n", current_player->locate[0], current_player->locate[1]);

    // Display hand cards
    printf("Hand: ");
    for (uint32_t i = 0; i < current_player->hand.SIZE; i++)
    {
        printf("%d ", current_player->hand.array[i]);
    }
    printf("\n");
}

bool handle_player_choice(game *gameState, int32_t choice)
{
    DEBUG_LOG("Processing player choice: State=%d, Choice=%d", gameState->status, choice);

    bool result = true;
    switch (gameState->status)
    {
    case CHOOSE_IDENTITY:
        if (choice < 1 || choice > 10)
        {
            ERROR_LOG("Invalid character choice: %d", choice);
            printf("Please choose a valid character (1-10)\n");
            return true;
        }
        handle_choose_identity(gameState, choice - 1); // Convert 1-based choice to 0-based CharacterID
        break;

    case CHOOSE_MOVE:
        if (choice < 0 || choice > 10)
        {
            ERROR_LOG("Invalid action choice: %d", choice);
            printf("Please choose a valid action (0-10)\n");
            return true;
        }
        handle_choose_move(gameState, choice);
        break;

    case USE_ATK:
    case USE_DEF:
    case USE_MOV:
        handle_use_card(gameState, choice);
        break;

    case BUY_CARD_TYPE:
        if (!handle_buy_card(gameState, choice))
        {
            ERROR_LOG("Failed to buy card: %d", choice);
            printf("Cannot buy this card, please choose again\n");
            return true;
        }
        break;

    default:
        ERROR_LOG("Unknown game state: %d", gameState->status);
        printf("Unknown game state!\n");
        return false;
    }

    INFO_LOG("Player choice processed");
    return result;
}

void handle_choose_identity(game *gameState, int8_t choice)
{
    DEBUG_LOG("Handling character choice: Player=%d, Character=%d",
              gameState->now_turn_player_id, choice);

    // Initialize the character for the current player
    init_character(gameState, gameState->now_turn_player_id, choice);

    INFO_LOG("Player %d chose character %d",
             gameState->now_turn_player_id + 1, choice);

    // Switch to next player or start game
    if (gameState->now_turn_player_id == 0)
    {
        // After Player 1 chooses, prompt Player 2
        gameState->now_turn_player_id = 1;
        printf("\nPlayer 2 Choose Character:\n");
        printf("1. Little Red Riding Hood\n2. Snow White\n3. Sleeping Beauty\n4. Alice\n");
        printf("5. Mulan\n6. Princess Kaguya\n7. The Little Mermaid\n8. The Little Match Girl\n");
        printf("9. Dorothy\n10. Scheherazade\n");
        gameState->status = CHOOSE_IDENTITY;
    }
    else
    {
        // After Player 2 chooses, start the game
        gameState->now_turn_player_id = 0;
        gameState->status = CHOOSE_MOVE;
        initial_draw(gameState);
    }
}

void handle_choose_move(game *gameState, int32_t choice)
{
    DEBUG_LOG("Handling action choice: Choice=%d", choice);

    bool valid_choice = true;
    switch (choice)
    {
    case 0: // Focus
        handle_focus_action(gameState);
        break;
    case 1: // Use Attack Card
        gameState->status = USE_ATK;
        break;
    case 2: // Use Defense Card
        gameState->status = USE_DEF;
        break;
    case 3: // Use Move Card
        gameState->status = USE_MOV;
        break;
    case 4: // Use Skill Card
        gameState->status = USE_SKILL;
        break;
    default:
        valid_choice = false;
        ERROR_LOG("Invalid action choice: %d", choice);
        printf("Please choose a valid action\n");
    }

    if (valid_choice)
    {
        INFO_LOG("Player chose action: %d", choice);
    }
}

void handle_use_card(game *gameState, int32_t cardIndex)
{
    if (cardIndex == 0)
    {
        // End current action
        gameState->status = CHOOSE_MOVE;
        return;
    }

    // Handle effects based on different card types
    switch (gameState->status)
    {
    case CHOOSE_IDENTITY:
    case CHOOSE_TENTACLE_LOCATION:
    case CHOOSE_SPECIAL_CARD:
    case APPEND_DESTINY_TOKEN:
    case SET_TARGET_LOCATE_TO_NEARBY:
    case CHOOSE_MOVE:
    case BUY_CARD_TYPE:
    case REMOVE_HG:
    case DROP_H:
        // These states do not handle card usage
        break;

    case USE_ATK:
        // Handle attack card
        gameState->nowATK += get_card_value(cardIndex);
        break;

    case USE_DEF:
        // Handle defense card
        gameState->nowDEF += get_card_value(cardIndex);
        break;

    case USE_MOV:
        // Handle move card
        gameState->nowMOV += get_card_value(cardIndex);
        break;

    case USE_POSION:
    case CHOOSE_MOVING_DIR:
    case USE_SKILL:
    case TRIGGER_COMBO:
    case USEBASIC:
    case KNOCKBACK:
    case MOVE_TARGET:
    case PUT_TARGET_POSITION:
    case SHUFFLE_POSION_TO_DECK:
    case CHOOSE_CARD_BACK:
    case SLEEP_ATK_HERTSELF:
    case USE_AWAKEN_TOKEN:
    case LOST_LIFE_FOR_USESKILL:
    case RECYCLE_CARD:
    case CHOOSECARDS:
    case TAKE_TO_HAND:
    case CHANGE_IDENTITY:
    case CHOOSE_MOVE_DIS:
    case SEND_CARD:
    case GET_KI:
    case SPEND_KI_FOR_ATK:
    case SPEND_KI_FOR_DRAW:
    case SPEND_KI_FOR_MOV:
    case DROP_ONE_DRAW_ONE:
    case PUT_TO_ANOTHER_SIDE:
    case CHOOSE_MOVE_NEARBY:
    case KEEP_OR_BACK:
    case LOST_LIFE_FOR_REMOVECARD:
    case KAGUYA_MOVE_TARGET:
    case MOVE_TO_TANTACLE:
    case CHOOSE_TANTACLE:
    case MOVE_TANTACLE:
    case DROPCARD_MOVE_TANTACLE:
    case SPEND_ENERGY:
    case SPEND_LIFE:
    case RECYCLE_MATCH:
    case DROP_CARD:
    case SPEND_COMBO:
    case FLIP_TOKEN_TO_RED:
    case CHOOSE_TOKEN:
    case TOKEN_GOAL:
    case GET_ULTRA:
    case USE_METAMORPHOSIS:
        // These states are not handled for now
        break;
    }
}

bool handle_buy_card(game *gameState, int32_t cardType)
{
    (void)cardType; // Mark parameter as intentionally unused
    if (!gameState)
        return false;

    // Handle card buying logic
    // TODO: Implement card buying logic

    gameState->status = CHOOSE_MOVE;
    return true;
}

bool check_game_end(game *gameState)
{
    // First verify both players have chosen characters
    if (gameState->status == CHOOSE_IDENTITY)
    {
        return false;
    }

    // Check if any player's HP is 0
    for (int i = 0; i < 2; i++)
    {
        // Only check HP if player has chosen a character (character != 0)
        if (gameState->players[i].character != 0 && gameState->players[i].life <= 0)
        {
            return true;
        }
    }
    return false;
}

void display_game_result(game *gameState)
{
    printf("\n=== Game Over ===\n");
    for (int i = 0; i < 2; i++)
    {
        printf("Player %d HP: %d\n", i + 1, gameState->players[i].life);
    }

    // Display winner
    int winner = -1;
    if (gameState->players[0].life <= 0)
    {
        winner = 1;
    }
    else if (gameState->players[1].life <= 0)
    {
        winner = 0;
    }

    if (winner >= 0)
    {
        printf("Player %d wins!\n", winner + 1);
    }
}
