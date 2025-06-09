#ifndef _GAME_INIT_H
#define _GAME_INIT_H

#include "architecture.h"

// 遊戲初始化
void init_game(game* gameState);

// 初始化玩家
void init_player(player* player);

// 初始化牌組
void init_decks(game* gameState);

// 初始化基本牌組
void init_basic_cards(game* gameState);

// 初始化特殊牌組
void init_special_cards(game* gameState);

#endif // _GAME_INIT_H
