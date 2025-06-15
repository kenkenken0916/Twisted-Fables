#ifndef _GAME_LOGIC_H
#define _GAME_LOGIC_H

#include "../utils/architecture.h"

/**
 * Process a single turn for the current player
 * @param gameState Pointer to game state
 * @return true if successful, false on error
 */
bool process_turn(game *gameState);

/**
 * Process the start phase of a turn
 * @param gameState Pointer to game state
 * @param playerIndex Current player index
 * @return true if successful, false on error
 */
bool process_start_phase(game *gameState, int playerIndex);

/**
 * Process the cleanup phase of a turn
 * @param gameState Pointer to game state
 * @param playerIndex Current player index
 * @return true if successful, false on error
 */
bool process_cleanup_phase(game *gameState, int playerIndex);

/**
 * Process the action phase of a turn
 * @param gameState Pointer to game state
 * @param playerIndex Current player index
 * @return true if successful, false on error
 */
bool process_action_phase(game *gameState, int playerIndex);

/**
 * Process the end phase of a turn
 * @param gameState Pointer to game state
 * @param playerIndex Current player index
 * @return true if successful, false on error
 */
bool process_end_phase(game *gameState, int playerIndex);

/**
 * Handle player choice based on current state
 * @param gameState Pointer to game state
 * @param choice Player's choice value
 * @return true if choice was valid and processed, false otherwise
 */
bool handle_player_choice(game *gameState, int32_t choice);

/**
 * Check if the game has ended
 * @param gameState Pointer to game state
 * @return true if game is over, false otherwise
 */
bool is_game_ended(game *gameState);

/**
 * Get the winner(s) of the game
 * @param gameState Pointer to game state
 * @param winners Array to store winner indices
 * @return Number of winners
 */
int get_winners(game *gameState, int *winners);

/**
 * Process attack action
 * @param gameState Pointer to game state
 * @param playerIndex Attacking player
 * @param cardIndices Array of card indices to use
 * @param cardCount Number of cards
 * @return true if successful, false on error
 */
bool process_attack_action(game *gameState, int playerIndex, int *cardIndices, int cardCount);

/**
 * Process defense action
 * @param gameState Pointer to game state
 * @param playerIndex Defending player
 * @param cardIndices Array of card indices to use
 * @param cardCount Number of cards
 * @return true if successful, false on error
 */
bool process_defense_action(game *gameState, int playerIndex, int *cardIndices, int cardCount);

/**
 * Process movement action
 * @param gameState Pointer to game state
 * @param playerIndex Moving player
 * @param cardIndices Array of card indices to use
 * @param cardCount Number of cards
 * @return true if successful, false on error
 */
bool process_movement_action(game *gameState, int playerIndex, int *cardIndices, int cardCount);

/**
 * Process skill usage
 * @param gameState Pointer to game state
 * @param playerIndex Player using skill
 * @param skillIndex Skill card index
 * @param basicIndex Basic card index
 * @return true if successful, false on error
 */
bool process_skill_action(game *gameState, int playerIndex, int skillIndex, int basicIndex);

/**
 * Process card purchase
 * @param gameState Pointer to game state
 * @param playerIndex Player buying card
 * @param cardType Type of card to buy
 * @return true if successful, false on error
 */
bool process_buy_action(game *gameState, int playerIndex, int cardType);

/**
 * Process focus action
 * @param gameState Pointer to game state
 * @param playerIndex Player focusing
 * @param targetIndex Index of card to remove
 * @param fromHand true if from hand, false if from discard
 * @return true if successful, false on error
 */
bool process_focus_action(game *gameState, int playerIndex, int targetIndex, bool fromHand);

// Additional helper functions
bool handle_action_phase(game *gameState);
bool handle_basic_card_usage(game *gameState);
bool handle_skill_usage(game *gameState);
bool handle_card_purchase(game *gameState);
bool handle_special_state(game *gameState);
bool handle_start_phase(game *gameState);
bool handle_cleanup_phase(game *gameState);
bool handle_end_phase(game *gameState);

// Utility functions
void show_available_actions(game *gameState);
void show_purchasable_cards(game *gameState);
bool validate_skill_usage(game *gameState);
bool execute_skill_effect(game *gameState);
bool process_attack_cards(game *gameState);
bool process_defense_cards(game *gameState);
bool process_movement_cards(game *gameState);
void handle_2v2_turn_order(game *gameState);
void resolve_ongoing_effects(game *gameState);
void discard_used_cards(player *currentPlayer);
void discard_hand(player *currentPlayer);
void draw_cards(player *currentPlayer, int count);
void shuffle_graveyard_to_deck(player *currentPlayer);
void resolve_end_turn_effects(game *gameState);

// Character-specific handlers
bool handle_alice_identity_choice(game *gameState);
bool handle_mermaid_tentacle_placement(game *gameState);
bool handle_sleeping_beauty_awaken(game *gameState);
bool handle_mulan_ki_generation(game *gameState);
bool handle_scheherazade_token_choice(game *gameState);

#endif // _GAME_LOGIC_H
