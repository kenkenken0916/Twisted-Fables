#ifndef _MAINLOOP_H
#define _MAINLOOP_H

#include "../utils/architecture.h"

/**
 * Main game loop and initialization
 * @param argc Command line argument count
 * @param argv Command line arguments
 * @return Exit code
 */
int mainloop(int argc, char *argv[]);

/**
 * Initialize game systems
 * @param gameState Pointer to game state
 * @return true if successful, false otherwise
 */
bool init_game_systems(game *gameState);

/**
 * Cleanup game systems
 * @param gameState Pointer to game state
 */
void cleanup_game_systems(game *gameState);

/**
 * Process one frame of the game loop
 * @param gameState Pointer to game state
 * @return true to continue, false to exit
 */
bool process_game_frame(game *gameState);

/**
 * Print the current state of the game, including all player information,
 * deck status, and character-specific details
 * @param gameState Pointer to the game state to print
 */
void print_game_state(const game *gameState);

#endif // _MAINLOOP_H
