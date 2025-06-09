#ifndef _TEST_SYSTEM_H
#define _TEST_SYSTEM_H

#include "architecture.h"
#include "card_system.h"
#include "character_system.h"
#include "game_state.h"
#include "game_init.h"
#include "utils.h"

// 測試結果結構
typedef struct {
    int total;
    int passed;
    int failed;
} TestResult;

// 初始化測試環境
void init_test_env(void);

// 執行所有測試
TestResult run_all_tests(void);

// 向量操作測試
void test_vector_operations(void);

// 卡牌系統測試
void test_card_system(void);

// 角色系統測試
void test_character_system(void);

// 遊戲狀態測試
void test_game_state(void);

// 戰鬥系統測試
void test_battle_system(void);

// 驗證函數
void assert_true(const char* test_name, bool condition);
void assert_equal_int(const char* test_name, int expected, int actual);
void assert_equal_vector(const char* test_name, vector* expected, vector* actual);

#endif // _TEST_SYSTEM_H
