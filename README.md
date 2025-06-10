# Twisted-Fables

2025 NTNU CSIE Computer Programming II Final Project

## 系統概述

Twisted Fables 是一個回合制卡牌對戰遊戲，基於童話故事角色設計。遊戲支援 1v1 和 2v2 模式，每個角色都有獨特的技能和卡牌組合。

## 系統架構

### 核心模組

1. **架構定義 (architecture.h)**
   - 定義遊戲的基本數據結構
   - 玩家狀態（生命值、防禦值、能量等）
   - 角色特殊能力結構
   - 遊戲狀態枚舉

2. **遊戲主循環 (mainloop.c)**
   - 遊戲初始化
   - 角色選擇階段
   - 主要遊戲循環
   - 狀態更新和轉換

3. **遊戲邏輯 (game_logic.c/h)**
   - 遊戲規則實現
   - 回合處理
   - 玩家行動處理
   - 勝利條件檢查

### 卡牌系統

1. **卡牌系統 (card_system.c/h)**
   - 卡牌定義和管理
   - 抽牌機制
   - 卡牌效果處理

2. **卡牌組合 (card_combination.c/h)**
   - 特殊卡牌組合效果
   - 連擊系統
   - 特殊技能觸發

### 角色系統

1. **角色系統 (character_system.c/h)**
   - 角色能力定義
   - 特殊技能實現
   - 被動效果處理

### 工具和輔助功能

1. **向量實現 (vector.c/h)**
   - 動態數組實現
   - 卡牌堆疊管理
   - 通用數據結構

2. **調試日誌 (debug_log.c/h)**
   - 日誌記錄系統
   - 調試信息輸出
   - 錯誤追蹤

3. **文本界面 (tui.c/h)**
   - 遊戲界面渲染
   - 用戶輸入處理
   - 遊戲狀態顯示

### 游戲流程

1. **初始化階段**
   - 系統初始化
   - 玩家數據設置
   - 卡牌系統初始化

2. **角色選擇階段**
   - 玩家選擇角色
   - 初始卡牌分配
   - 特殊能力設置

3. **主要遊戲循環**
   - 回合開始階段
   - 行動階段（出牌、使用技能等）
   - 回合結束處理
   - 狀態更新

4. **遊戲結束處理**
   - 勝利條件檢查
   - 結果顯示
   - 資源清理

## 編譯和運行

### 編譯選項

- `make all`: 構建遊戲和測試程式
- `make clean`: 清理所有生成的檔案
- `make test`: 運行測試
- `make run`: 運行遊戲
- `make game`: 只構建遊戲
- `make debug`: 構建調試版本
- `make release`: 構建優化的發布版本

### 開發工具

- 編譯器：GCC
- 編譯標準：C11
- 警告級別：嚴格 (-Wall -Wextra -Werror)

## 測試系統

1. **測試框架 (test_system.c/h)**
   - 單元測試實現
   - 測試用例管理
   - 結果報告生成

2. **測試執行 (test_main.c)**
   - 測試套件運行
   - 錯誤報告
   - 覆蓋率統計

## 角色列表

1. 小紅帽 (Little Red Riding Hood)
2. 白雪公主 (Snow White)
3. 睡美人 (Sleeping Beauty)
4. 愛麗絲 (Alice)
5. 花木蘭 (Mulan)
6. 輝夜姬 (Princess Kaguya)
7. 美人魚 (The Little Mermaid)
8. 賣火柴女孩 (The Little Match Girl)
9. 桃樂絲 (Dorothy)
10. 山魯佐德 (Scheherazade)

每個角色都有獨特的特殊能力和遊戲機制，詳細規則請參考 `gamerule.md`。
