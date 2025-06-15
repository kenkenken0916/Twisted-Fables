#ifndef _STATE_SYSTEM_H
#define _STATE_SYSTEM_H

#include "../utils/architecture.h"

// Input handler prototypes for each state
int8_t handle_choose_identity(game *gameState);
int32_t handle_choose_tentacle_location(game *gameState);
int32_t handle_choose_special_card(game *gameState);
int32_t handle_append_destiny_token(game *gameState);
int32_t handle_set_target_locate_to_nearby(game *gameState);
int32_t handle_choose_move(game *gameState, int32_t *returnValue);
int32_t handle_buy_card_type(game *gameState);
int32_t handle_remove_hg(game *gameState,int32_t* returnValue);
int32_t handle_drop_h(game *gameState);
int32_t handle_use_atk(game *gameState, int32_t *returnValue);
int32_t handle_use_def(game *gameState);
int32_t handle_use_mov(game *gameState);
int32_t handle_use_posion(game *gameState);
int8_t handle_choose_moving_dir(game *gameState);
int32_t handle_use_skill(game *gameState);
int8_t handle_trigger_combo(game *gameState);
int32_t handle_usebasic(game *gameState);
int32_t handle_knockback(game *gameState);
int32_t handle_move_target(game *gameState);
int32_t handle_put_target_position(game *gameState);
int8_t handle_shuffle_posion_to_deck(game *gameState);
int32_t handle_choose_card_back(game *gameState);
int32_t handle_sleep_atk_hertself(game *gameState);
int8_t handle_use_awaken_token(game *gameState);
int8_t handle_lost_life_for_useskill(game *gameState);
int32_t handle_recycle_card(game *gameState);
int32_t handle_choosecards(game *gameState);
int8_t handle_take_to_hand(game *gameState);
int8_t handle_change_identity(game *gameState);
int8_t handle_choose_move_dis(game *gameState);
int32_t handle_send_card(game *gameState);
int32_t handle_get_ki(game *gameState);
int32_t handle_spend_ki_for_atk(game *gameState);
int32_t handle_spend_ki_for_draw(game *gameState);
int32_t handle_spend_ki_for_mov(game *gameState);
int32_t handle_drop_one_draw_one(game *gameState);
int8_t handle_put_to_another_side(game *gameState);
int8_t handle_choose_move_nearby(game *gameState);
int8_t handle_keep_or_back(game *gameState);
int8_t handle_lost_life_for_removecard(game *gameState);
int8_t handle_kaguya_move_target(game *gameState);
int32_t handle_move_to_tantacle(game *gameState);
int32_t handle_choose_tantacle(game *gameState);
int32_t handle_move_tantacle(game *gameState);
int32_t handle_dropcard_move_tantacle(game *gameState);
int32_t handle_spend_energy(game *gameState);
int32_t handle_spend_life(game *gameState);
int32_t handle_recycle_match(game *gameState);
int32_t handle_drop_card(game *gameState);
int32_t handle_spend_combo(game *gameState);
int8_t handle_flip_token_to_red(game *gameState);
int8_t handle_choose_token(game *gameState);
int8_t handle_token_goal(game *gameState);
int8_t handle_get_ultra(game *gameState);
int32_t handle_use_metamorphosis(game *gameState);

// Helper functions
void clear_input_buffer(void);
bool validate_int_input(const char *input, int *value);

#endif // _STATE_SYSTEM_H
