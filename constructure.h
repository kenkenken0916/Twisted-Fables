#ifndef _CONSTRUCTURE_H
#define _CONSTRUCTURE_H

#include <limits.h>
#include <math.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "vector.h"

#define WIDTH 9

typedef struct _player {
    int8_t team;
    uint8_t locate[2]; 
    uint8_t character;
    uint8_t maxlife;
    uint8_t life;
    uint8_t maxdefense;
    uint8_t defense;
    uint8_t energy;
    uint8_t specialGate;
    vector hand;
    vector deck;
    vector usecards;
    vector graveyard;
    vector metamorphosis;
    vector attackSkill;
    vector defenseSkill;
    vector moveSkill;
    vector specialDeck;
    // Little Red Riding Hood 0
    struct {
        int32_t saveCard[3];
    } redHood;

    // Snow White 1
    struct {
        vector remindPosion;
    } snowWhite;

    // sleeping Beauty 2
    struct {
        uint32_t AWAKEN_TOKEN;
        int8_t AWAKEN;  // 1:醒著 0:沉睡
        int8_t dayNightmareDrawRemind;
        int32_t atkRise;
        int32_t atkRiseTime;
        int8_t usedmeta1;
    } sleepingBeauty;

    // alice 3
    struct {
        uint8_t identity;  // 0:無, 1:紅心皇后, 2:瘋帽子, 3:柴郡貓
        int32_t riseBasic;
        int32_t restartTurn;
        int32_t havedrestart;
    } alice;

    // Mulan 4
    struct {
        uint32_t KI_TOKEN;
        uint8_t extraCard;
        uint8_t extraDraw;
    } mulan;

    // kaguya 5
    struct {
        int8_t useDefenseAsATK;
        int8_t useMoveTarget;
    } kaguya;

    // mermaid 6
    struct {
    } mermaid;

    // Match Girl 7
    struct {
        uint32_t remindMatch;
        uint32_t pushedMatch;
    } matchGirl;

    // dorothy 8
    struct {
        uint32_t COMBO_TOKEN;
        int8_t canCombo;
    } dorothy;

    // Scheherazade 9
    struct {
        vector destiny_TOKEN_locate;
        vector destiny_TOKEN_type;  // 1:blue, 2:red
        int8_t selectToken;
    } scheherazade;
} Player;

enum state {
    CHOOSE_IDENTITY = 0,
    CHOOSE_TENTACLE_LOCATION,
    CHOOSE_SPECIAL_CARD,
    APPEND_DESTINY_TOKEN,
    SET_TARGET_LOCATE_TO_NEARBY,
    CHOOSE_MOVE,
    BUY_CARD_TYPE,
    REMOVE_HG,
    DROP_H,
    USE_ATK,
    USE_DEF,
    USE_MOV,
    USE_POSION,
    CHOOSE_MOVING_DIR,
    USE_SKILL,
    TRIGGER_COMBO,
    USEBASIC,
    KNOCKBACK,
    MOVE_TARGET,
    PUT_TARGET_POSITION,
    SHUFFLE_POSION_TO_DECK,
    CHOOSE_CARD_BACK,
    SLEEP_ATK_HERTSELF,
    USE_AWAKEN_TOKEN,
    LOST_LIFE_FOR_USESKILL,
    RECYCLE_CARD,
    CHOOSECARDS,
    TAKE_TO_HAND,
    CHANGE_IDENTITY,
    CHOOSE_MOVE_DIS,
    SEND_CARD,
    GET_KI,
    SPEND_KI_FOR_ATK,
    SPEND_KI_FOR_DRAW,
    SPEND_KI_FOR_MOV,
    DROP_ONE_DRAW_ONE,
    PUT_TO_ANOTHER_SIDE,
    CHOOSE_MOVE_NEARBY,
    KEEP_OR_BACK,
    LOST_LIFE_FOR_REMOVECARD,
    KAGUYA_MOVE_TARGET,
    MOVE_TO_TANTACLE,
    CHOOSE_TANTACLE,
    MOVE_TANTACLE,
    DROPCARD_MOVE_TANTACLE,
    SPEND_ENERGY,
    SPEND_LIFE,
    RECYCLE_MATCH,
    DROP_CARD,
    SPEND_COMBO,
    FLIP_TOKEN_TO_RED,
    CHOOSE_TOKEN,
    TOKEN_GOAL,
    GET_ULTRA,
    USE_METAMORPHOSIS,
};

// TUI
#include "tui.h"
#include <ncurses.h>
#include <unistd.h>
#include <signal.h>

extern volatile sig_atomic_t running;

void handle_sigint(int sig);
int tui_init(TUI *tui);
void tui_cleanup(TUI *tui);
void draw_event_log(WINDOW *win, const char *log[], int log_size);
void draw_positions(WINDOW *win, Player p1, Player p2);
void draw_stats(WINDOW *win, Player p1, Player p2);
void tui_update(TUI *tui, Player p1, Player p2, const char *logs[], int log_size);

#endif
