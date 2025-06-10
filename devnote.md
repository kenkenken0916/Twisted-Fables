# Twisted Fables 開發文件

## 檔案結構與功能說明

### 1. 核心系統檔案

#### architecture.h
系統核心架構定義
- 玩家結構體 `struct _player`
  - 基礎屬性（生命值、防禦值、能量等）
  - 各個角色特殊屬性
- 遊戲狀態枚舉 `enum state`
- 向量類型定義

#### mainloop.c
主遊戲循環
- `int main(void)` - 程式入口點
- 遊戲初始化
- 玩家選擇處理
- 回合循環控制

### 2. 遊戲邏輯模組

#### game_logic.c/h
遊戲核心邏輯
- `bool handle_player_choice(game* gameState, int32_t choice)` - 處理玩家選擇
- `void transition_to_next_state(game* gameState)` - 狀態轉換
- `bool check_game_end(game* gameState)` - 檢查遊戲結束條件

#### game_init.c/h
遊戲初始化
- `void init_game(game* gameState)` - 遊戲狀態初始化
- `void setup_players(game* gameState)` - 玩家設置
- `void init_decks(game* gameState)` - 牌組初始化

#### game_state.c/h
遊戲狀態管理
- `void update_game_state(game* gameState)` - 更新遊戲狀態
- `void display_game_state(game* gameState)` - 顯示遊戲狀態
- `void save_game_state(game* gameState)` - 保存遊戲狀態

### 3. 卡牌系統

#### card_system.c/h
卡牌系統核心
- `void init_card_system(game* gameState)` - 初始化卡牌系統
- `int32_t get_card_value(int32_t cardId)` - 獲取卡牌數值
- `bool needs_awakening(int32_t skillCard)` - 檢查覺醒需求
- `bool needs_ki(int32_t skillCard)` - 檢查氣力需求
- `int32_t get_skill_cost(int32_t skillCard)` - 獲取技能消耗

#### card_combination.c/h
卡牌組合效果
- `bool check_combination(vector* cards)` - 檢查卡牌組合
- `void apply_combination_effect(game* gameState, vector* cards)` - 應用組合效果
- `void handle_chain_effect(game* gameState)` - 處理連鎖效果

### 4. 角色系統

#### character_system.c/h
角色系統實現
- `void init_character(game* gameState, int playerIndex, CharacterID charId)` - 初始化角色
- `bool handle_character_skill(game* gameState, int32_t skillCard)` - 處理角色技能
- `void handle_character_passive(game* gameState)` - 處理被動效果
- 各角色專屬技能函數（如 `red_hood_skill`, `snow_white_skill` 等）

### 5. 工具和輔助功能

#### vector.c/h
動態數組實現
- `void initVector(vector* vec)` - 初始化向量
- `void vector_pushback(vector* vec, int32_t val)` - 添加元素
- `void vector_popback(vector* vec)` - 移除元素
- `void eraseVector(vector* vec, uint32_t index)` - 清除指定位置元素
- `void shuffle_deck(vector* vec)` - 洗牌功能

#### debug_log.c/h
調試日誌系統
- `void DEBUG_LOG(const char* format, ...)` - 調試信息輸出
- `void ERROR_LOG(const char* format, ...)` - 錯誤信息輸出
- `void INFO_LOG(const char* format, ...)` - 一般信息輸出

#### utils.c/h
通用工具函數
- `bool draw_card(player* p, int count)` - 抽牌功能
- `void adjust_life(player* p, int32_t amount)` - 調整生命值
- `void adjust_defense(player* p, int32_t amount)` - 調整防禦值
- `bool has_enough_energy(player* p, int cost)` - 檢查能量是否足夠

#### tui.c/h
文字用戶界面
- `void draw_game_screen(game* gameState)` - 繪製遊戲畫面
- `void draw_battlefield(game* gameState)` - 繪製戰場
- `void draw_player_info(player* p)` - 顯示玩家信息

### 6. 測試系統

#### test_system.c/h
測試框架
- `TestResult run_all_tests()` - 運行所有測試
- `void test_card_system()` - 測試卡牌系統
- `void test_character_system()` - 測試角色系統
- `void test_game_logic()` - 測試遊戲邏輯

#### test_main.c
測試入口
- `int main(void)` - 測試程式入口點
- 測試結果輸出
- 錯誤報告生成

## 開發注意事項

1. 編碼規範
   - 使用 C11 標準
   - 遵循嚴格的警告標準
   - 保持代碼整潔和一致性

2. 錯誤處理
   - 所有函數都應該進行適當的錯誤檢查
   - 使用 DEBUG_LOG 記錄重要操作
   - 使用 ERROR_LOG 記錄錯誤情況

3. 記憶體管理
   - 注意動態分配的記憶體釋放
   - 避免記憶體洩漏
   - 使用向量結構管理動態數組

4. 測試要求
   - 新增功能時需要添加對應的測試用例
   - 修改功能時需要確保所有測試通過
   - 定期運行完整測試套件

5. 版本控制
   - 使用 git 管理代碼
   - 提交前確保代碼能夠編譯
   - 保持提交信息清晰和有意義

## 擴展建議

1. 新增角色
   - 在 `character_system.h` 添加角色 ID
   - 實現角色特殊技能
   - 添加對應的測試用例

2. 新增卡牌
   - 在 `card_system.h` 定義新卡牌
   - 實現卡牌效果
   - 更新卡牌組合系統

3. 遊戲模式
   - 可以在 `game_logic.c` 中添加新的遊戲模式
   - 注意平衡性調整
   - 確保與現有系統兼容
