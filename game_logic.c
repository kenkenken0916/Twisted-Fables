#include <stdio.h>
#include <stdlib.h>
#include "game_logic.h"
#include "debug_log.h"
#include "card_combination.h"
#include "architecture.h"
#include "character_system.h"

static void display_character_selection(void)
{
    printf("Choose Character:\n");
    printf("1. Little Red Riding Hood\n2. Snow White\n3. Sleeping Beauty\n4. Alice\n");
    printf("5. Mulan\n6. Princess Kaguya\n7. The Little Mermaid\n8. The Little Match Girl\n");
    printf("9. Dorothy\n10. Scheherazade\n");
}

void display_game_state(game *gs)
{
    static int8_t last_player = -1;
    static enum state last_state = CHOOSE_IDENTITY;

    // Only update display if player or state has changed
    if (last_player == gs->now_turn_player_id && last_state == gs->status)
    {
        return;
    }

    // Character selection phase
    if (gs->status == CHOOSE_IDENTITY)
    {
        printf("\n=== Player %d's Turn ===\n", gs->now_turn_player_id + 1);
        if (gs->players[0].character == 0)
        {
            display_character_selection();
        }
        else if (gs->now_turn_player_id == 1 && gs->players[1].character == 0)
        {
            printf("\nPlayer 2's turn\n");
            display_character_selection();
        }
        last_player = gs->now_turn_player_id;
        last_state = gs->status;
        return;
    }

    // For other game states, show full game state
    INFO_LOG("Display Game State - Current Player: %d", gs->now_turn_player_id + 1);
    printf("\n=== Game State ===\n");
    printf("Current Player: %d\n", gs->now_turn_player_id + 1);

    // Display player information
    player *current_player = &gs->players[gs->now_turn_player_id];
    printf("HP: %d/%d\n", current_player->life, current_player->maxlife);
    printf("Defense: %d/%d\n", current_player->defense, current_player->maxdefense);
    printf("Energy: %d\n", current_player->energy);
    printf("Position: %d,%d\n", current_player->locate[0], current_player->locate[1]);

    // Display hand cards
    printf("Hand: ");
    for (uint32_t i = 0; i < current_player->hand.SIZE; i++)
    {
        int32_t cardId = current_player->hand.array[i];
        printf("%s ", get_card_name(cardId));
    }
    printf("\n");

    // Display current state with description
    printf("\nCurrent State: ");
    switch (gs->status)
    {
    case CHOOSE_IDENTITY:
        printf("選擇角色");
        break;
    case CHOOSE_MOVE:
        printf("選擇行動 (0:專注 1:攻擊 2:防禦 3:移動 4:技能 5:特殊 6:購買 7:蛻變 8:特殊行動 9:丟棄毒牌 10:結束)");
        break;
    case USE_ATK:
        printf("使用攻擊牌 (輸入卡牌編號1~4，0結束)");
        break;
    case USE_DEF:
        printf("使用防禦牌 (輸入卡牌編號1~4，0結束)");
        break;
    case USE_MOV:
        printf("使用移動牌 (輸入卡牌編號1~4，0結束)");
        break;
    case USE_SKILL:
        printf("使用技能牌 (輸入卡牌編號1~4，0結束)");
        break;
    case BUY_CARD_TYPE:
        printf("選擇要購買的牌類型 (-3:技能移動 -2:技能防禦 -1:技能攻擊 1~3:攻擊 4~6:防禦 7~9:移動 10:通用)");
        break;
    case USEBASIC:
        printf("選擇基本牌搭配技能 (輸入卡牌編號1~4，0結束)");
        break;
    default:
        printf("狀態 %d", gs->status);
        break;
    }
    printf("\n");
}

void display_available_cards(game *gs, bool (*card_filter)(int32_t))
{
    player *current = &gs->players[gs->now_turn_player_id];
    printf("Available cards:\n");
    for (uint32_t i = 0; i < current->hand.SIZE; i++)
    {
        int32_t cardId = current->hand.array[i];
        if (card_filter(cardId))
        {
            printf("%d: %s\n", i + 1, get_card_name(cardId));
        }
    }
}

bool handle_player_choice(game *gs, int32_t player_choice)
{
    DEBUG_LOG("Processing player choice: State=%d, Choice=%d", gs->status, player_choice);

    bool result = true;
    switch (gs->status)
    {
    case CHOOSE_IDENTITY:
        if (player_choice < 1 || player_choice > 10)
        {
            ERROR_LOG("Invalid character choice: %d", player_choice);
            printf("Please choose a valid character (1-10)\n");
            return true;
        }
        handle_choose_identity(gs, player_choice - 1); // Convert 1-based choice to 0-based CharacterID
        break;

    case CHOOSE_MOVE:
        if (player_choice < 0 || player_choice > 10)
        {
            ERROR_LOG("Invalid action choice: %d", player_choice);
            printf("Please choose a valid action (0-10)\n");
            return true;
        }
        handle_choose_move(gs, player_choice);
        break;

    case USE_ATK:
        // Show attack cards
        if (player_choice == 0)
        {
            gs->status = CHOOSE_MOVE;
        }
        else
        {
            player *current = &gs->players[gs->now_turn_player_id];
            if (player_choice > 0 && player_choice <= (int32_t)current->hand.SIZE)
            {
                handle_use_card(gs, player_choice);
            }
            else
            {
                printf("Invalid card choice. Please choose a number between 0 and %d\n", current->hand.SIZE);
            }
        }
        break;

    case USE_DEF:
        // Show defense cards
        if (player_choice == 0)
        {
            gs->status = CHOOSE_MOVE;
        }
        else
        {
            player *current = &gs->players[gs->now_turn_player_id];
            if (player_choice > 0 && player_choice <= (int32_t)current->hand.SIZE)
            {
                handle_use_card(gs, player_choice);
            }
            else
            {
                printf("Invalid card choice. Please choose a number between 0 and %d\n", current->hand.SIZE);
            }
        }
        break;

    case BUY_CARD_TYPE:
        if (!handle_buy_card(gs, player_choice))
        {
            ERROR_LOG("Failed to buy card: %d", player_choice);
            printf("Cannot buy this card, please choose again\n");
            return true;
        }
        break;

    default:
        ERROR_LOG("Unknown game state: %d", gs->status);
        printf("Unknown game state!\n");
        return false;
    }

    INFO_LOG("Player choice processed");
    return result;
}

void handle_choose_identity(game *gs, int8_t ch)
{
    DEBUG_LOG("Handling character choice: Player=%d, Character=%d",
              gs->now_turn_player_id, ch);

    // Add a flag in the player struct to track if character has been selected
    if (gs->players[gs->now_turn_player_id].character != UINT8_MAX)
    {
        return;
    }

    // Initialize the character for the current player
    init_character(gs, gs->now_turn_player_id, ch);

    // Verify character was initialized by checking if it's no longer UINT8_MAX
    if (gs->players[gs->now_turn_player_id].character == UINT8_MAX)
    {
        ERROR_LOG("Character initialization failed for player %d", gs->now_turn_player_id + 1);
        printf("Character selection failed\n");
        return;
    }

    INFO_LOG("Player %d chose character %d", gs->now_turn_player_id + 1, ch);

    if (gs->now_turn_player_id == 0)
    {
        // After Player 1 successfully chooses, switch to Player 2
        gs->now_turn_player_id = 1;
    }
    else if (gs->now_turn_player_id == 1)
    {
        // After Player 2 chooses, start the game
        gs->status = CHOOSE_MOVE;
        gs->now_turn_player_id = 0;
        initial_draw(gs);
    }
}

void handle_choose_move(game *gs, int32_t move_choice)
{
    DEBUG_LOG("Handling action choice: Choice=%d", move_choice);

    bool valid_choice = true;
    switch (move_choice)
    {
    case 0: // Focus
        handle_focus_action(gs);
        break;
    case 1: // Use Attack Card
        gs->status = USE_ATK;
        // Display available attack cards
        printf("Choose attack card to use (1-%d, 0 to cancel):\n", gs->players[gs->now_turn_player_id].hand.SIZE);
        break;
    case 2: // Use Defense Card
        gs->status = USE_DEF;
        // Display available defense cards
        printf("Choose defense card to use (1-%d, 0 to cancel):\n", gs->players[gs->now_turn_player_id].hand.SIZE);
        break;
    case 3: // Use Move Card
        gs->status = USE_MOV;
        // Display available move cards
        printf("Choose move card to use (1-%d, 0 to cancel):\n", gs->players[gs->now_turn_player_id].hand.SIZE);
        break;
    case 4: // Use Skill Card
        gs->status = USE_SKILL;
        // Display available skill cards
        printf("Choose skill card to use (1-%d, 0 to cancel):\n", gs->players[gs->now_turn_player_id].hand.SIZE);
        break;
    case 10: // End turn
        DEBUG_LOG("Ending turn for player %d", gs->now_turn_player_id + 1);
        end_turn(gs);
        break;
    default:
        valid_choice = false;
        ERROR_LOG("Invalid action choice: %d", move_choice);
        printf("Please choose a valid action\n");
        return;
    }

    if (valid_choice)
    {
        INFO_LOG("Player chose action: %d", move_choice);
    }
}

void handle_use_card(game *gs, int32_t card_idx)
{
    if (card_idx == 0)
    {
        // End current action
        gs->status = CHOOSE_MOVE;
        return;
    }

    player *current = &gs->players[gs->now_turn_player_id];

    // Validate card index
    if (card_idx < 1 || card_idx > (int32_t)current->hand.SIZE)
    {
        printf("Invalid card index\n");
        return;
    }

    // Get the actual card from hand
    int32_t cardId = current->hand.array[card_idx - 1];
    CardType type = get_card_type(cardId);
    bool valid_card = false;

    // Handle effects based on different card types
    switch (gs->status)
    {
    case USE_ATK:
        if (type == CARD_TYPE_BASIC_ATK)
        {
            gs->nowATK += get_card_value(cardId);
            valid_card = true;
        }
        else
        {
            printf("This is not an attack card\n");
        }
        break;

    case USE_DEF:
        if (type == CARD_TYPE_BASIC_DEF)
        {
            gs->nowDEF += get_card_value(cardId);
            valid_card = true;
        }
        else
        {
            printf("This is not a defense card\n");
        }
        break;

    case USE_MOV:
        if (type == CARD_TYPE_BASIC_MOV)
        {
            gs->nowMOV += get_card_value(cardId);
            valid_card = true;
        }
        else
        {
            printf("This is not a move card\n");
        }
        break;

    default:
        // Other states are not handled for now
        break;
    }

    if (valid_card)
    {
        // Record the used card
        gs->nowUsingCardID = cardId;
        // Move card from hand to used cards
        vector_pushback(&current->usecards, cardId);
        eraseVector(&current->hand, card_idx - 1);
        // Return to choose move state
        gs->status = CHOOSE_MOVE;
    }
}

bool handle_buy_card(game *gs, int32_t card_type)
{
    if (!gs)
        return false;

    // Handle card buying logic
    // TODO: Implement card buying logic

    gs->status = CHOOSE_MOVE;
    return true;
}

bool check_game_end(game *gs)
{
    // First verify both players have chosen characters
    if (gs->status == CHOOSE_IDENTITY)
    {
        return false;
    }

    // Don't end game until both players have selected characters
    if (gs->players[0].character == UINT8_MAX || gs->players[1].character == UINT8_MAX)
    {
        return false;
    }

    // Check if any player's HP is 0
    for (int i = 0; i < 2; i++)
    {
        if (gs->players[i].life <= 0)
        {
            return true;
        }
    }
    return false;
}

void display_game_result(game *gs)
{
    printf("\n=== Game Over ===\n");
    for (int i = 0; i < 2; i++)
    {
        printf("Player %d HP: %d\n", i + 1, gs->players[i].life);
    }

    // Display winner
    int winner = -1;
    if (gs->players[0].life <= 0)
    {
        winner = 1;
    }
    else if (gs->players[1].life <= 0)
    {
        winner = 0;
    }

    if (winner >= 0)
    {
        printf("Player %d wins!\n", winner + 1);
    }
}

// Card filter functions
bool is_attack_card(int32_t cardId)
{
    return get_card_type(cardId) == CARD_TYPE_BASIC_ATK;
}

bool is_defense_card(int32_t cardId)
{
    return get_card_type(cardId) == CARD_TYPE_BASIC_DEF;
}

bool is_move_card(int32_t cardId)
{
    return get_card_type(cardId) == CARD_TYPE_BASIC_MOV;
}

bool is_skill_card(int32_t cardId)
{
    CardType type = get_card_type(cardId);
    return type == CARD_TYPE_SKILL_ATK ||
           type == CARD_TYPE_SKILL_DEF ||
           type == CARD_TYPE_SKILL_MOV;
}
