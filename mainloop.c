#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
// #include "tui.h"  // Commented out for now to use command line interface
#include "architecture.h"
#include "character_system.h"
#include "game_init.h"
#include "game_logic.h"
#include "card_system.h"
#include "game_state.h"

int main(void)
{
    // Initialize game
    game gameState;
    init_game(&gameState);

    // Initialize card system
    init_card_system(&gameState);

    printf("=== Twisted Fables ===\n");
    printf("1v1 Mode\n");

    // Character selection phase
    printf("\nPlayer 1 Choose Character:\n");
    printf("1. Little Red Riding Hood\n2. Snow White\n3. Sleeping Beauty\n4. Alice\n");
    printf("5. Mulan\n6. Princess Kaguya\n7. The Little Mermaid\n8. The Little Match Girl\n");
    printf("9. Dorothy\n10. Scheherazade\n");

    // Main game loop
    while (1)
    {
        // Display current game state
        display_game_state(&gameState);

        // Request player input based on current state
        int32_t choice;
        printf("\nCurrent State: %d\n", gameState.status);
        printf("Enter your choice: ");
        scanf("%d", &choice);

        // Handle player choice
        if (!handle_player_choice(&gameState, choice))
        {
            break; // Game over or error
        }

        // Update game state
        transition_to_next_state(&gameState);

        // Check if game has ended
        if (check_game_end(&gameState))
        {
            break;
        }
    }

    // Display game results
    display_game_result(&gameState);

    return 0;
}
