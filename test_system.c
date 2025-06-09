#include <stdio.h>
#include <string.h>
#include "test_system.h"
#include "debug_log.h"

static TestResult test_result;

void init_test_env(void) {
    test_result.total = 0;
    test_result.passed = 0;
    test_result.failed = 0;
    
    // 初始化日誌系統
    init_logging("test.log");
    INFO_LOG("開始測試");
}

void assert_true(const char* test_name, bool condition) {
    test_result.total++;
    
    if (condition) {
        test_result.passed++;
        printf("✓ %s\n", test_name);
        DEBUG_LOG("測試通過：%s", test_name);
    } else {
        test_result.failed++;
        printf("✗ %s\n", test_name);
        ERROR_LOG("測試失敗：%s", test_name);
    }
}

void assert_equal_int(const char* test_name, int expected, int actual) {
    test_result.total++;
    
    if (expected == actual) {
        test_result.passed++;
        printf("✓ %s (期望=%d, 實際=%d)\n", test_name, expected, actual);
        DEBUG_LOG("測試通過：%s (期望=%d, 實際=%d)", test_name, expected, actual);
    } else {
        test_result.failed++;
        printf("✗ %s (期望=%d, 實際=%d)\n", test_name, expected, actual);
        ERROR_LOG("測試失敗：%s (期望=%d, 實際=%d)", test_name, expected, actual);
    }
}

void assert_equal_vector(const char* test_name, vector* expected, vector* actual) {
    test_result.total++;
    
    if (expected->SIZE != actual->SIZE) {
        test_result.failed++;
        printf("✗ %s (長度不匹配：期望=%u, 實際=%u)\n", 
               test_name, expected->SIZE, actual->SIZE);
        ERROR_LOG("測試失敗：%s (長度不匹配：期望=%u, 實際=%u)", 
                 test_name, expected->SIZE, actual->SIZE);
        return;
    }
    
    for (uint32_t i = 0; i < expected->SIZE; i++) {
        if (expected->array[i] != actual->array[i]) {
            test_result.failed++;
            printf("✗ %s (索引%u不匹配：期望=%d, 實際=%d)\n",
                   test_name, i, expected->array[i], actual->array[i]);
            ERROR_LOG("測試失敗：%s (索引%u不匹配：期望=%d, 實際=%d)",
                     test_name, i, expected->array[i], actual->array[i]);
            return;
        }
    }
    
    test_result.passed++;
    printf("✓ %s\n", test_name);
    DEBUG_LOG("測試通過：%s", test_name);
}

void test_vector_operations(void) {
    printf("\n=== 測試向量操作 ===\n");
    
    // 初始化測試
    vector vec = initVector();
    assert_true("向量初始化", vec.SIZE == 0);
    
    // 插入測試
    pushbackVector(&vec, 1);
    pushbackVector(&vec, 2);
    pushbackVector(&vec, 3);
    assert_equal_int("向量大小", 3, vec.SIZE);
    assert_equal_int("第一個元素", 1, vec.array[0]);
    
    // 刪除測試
    popbackVector(&vec);
    assert_equal_int("刪除後大小", 2, vec.SIZE);
    
    // 清空測試
    clearVector(&vec);
    assert_true("清空後大小", vec.SIZE == 0);
}

void test_card_system(void) {
    printf("\n=== 測試卡牌系統 ===\n");
    
    game gameState;
    init_game(&gameState);
    
    // 測試基本牌初始化
    assert_true("攻擊牌組初始化", 
                gameState.basicBuyDeck[0][0].SIZE > 0);
    
    // 測試抽牌
    player* p = &gameState.players[0];
    int initial_size = p->hand.SIZE;
    draw_card(p, 1);
    assert_equal_int("抽牌後手牌數", initial_size + 1, p->hand.SIZE);
    
    // 測試使用卡牌
    int32_t card_id = p->hand.array[0];
    use_card(&gameState, card_id);
    assert_true("使用卡牌後進入出牌區", 
                findVector(&p->usecards, card_id) != -1);
}

void test_character_system(void) {
    printf("\n=== 測試角色系統 ===\n");
    
    game gameState;
    init_game(&gameState);
    
    // 測試角色初始化
    init_character(&gameState, 0, CHAR_RED_HOOD);
    player* p = &gameState.players[0];
    assert_equal_int("小紅帽生命上限", 30, p->maxlife);
    assert_equal_int("小紅帽防禦上限", 6, p->maxdefense);
    
    // 測試技能卡初始化
    assert_true("技能牌初始化", p->attackSkill.SIZE > 0);
}

void test_game_state(void) {
    printf("\n=== 測試遊戲狀態 ===\n");
    
    game gameState;
    init_game(&gameState);
    
    // 測試回合切換
    int initial_player = gameState.now_turn_player_id;
    end_turn(&gameState);
    assert_equal_int("回合切換", 
                    (initial_player + 1) % 2, 
                    gameState.now_turn_player_id);
                    
    // 測試狀態轉換
    gameState.status = CHOOSE_IDENTITY;
    transition_to_next_state(&gameState);
    assert_true("狀態轉換", 
                gameState.status != CHOOSE_IDENTITY);
}

void test_battle_system(void) {
    printf("\n=== 測試戰鬥系統 ===\n");
    
    game gameState;
    init_game(&gameState);
    
    // 初始化玩家位置
    gameState.players[0].locate[0] = 0;
    gameState.players[0].locate[1] = 0;
    gameState.players[1].locate[0] = 1;
    gameState.players[1].locate[1] = 1;
    
    // 測試距離計算
    int distance = calculate_distance(0, 0, 1, 1);
    assert_equal_int("距離計算", 2, distance);
    
    // 測試射程檢查
    assert_true("射程1檢查", 
                !is_in_range(&gameState, 0, 1, 1));
    assert_true("射程2檢查", 
                is_in_range(&gameState, 0, 1, 2));
}

TestResult run_all_tests(void) {
    init_test_env();
    
    test_vector_operations();
    test_card_system();
    test_character_system();
    test_game_state();
    test_battle_system();
    
    printf("\n=== 測試結果 ===\n");
    printf("總計: %d\n", test_result.total);
    printf("通過: %d\n", test_result.passed);
    printf("失敗: %d\n", test_result.failed);
    
    return test_result;
}
