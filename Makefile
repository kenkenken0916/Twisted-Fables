# 編譯器設置
CC = gcc
# 編譯警告選項
WARNINGS = -Wall -Wextra -Werror -Wformat=2 -Wno-unused-parameter \
           -Wshadow -Wwrite-strings -Wstrict-prototypes -Wold-style-definition \
           -Wredundant-decls -Wnested-externs -Wmissing-include-dirs

# 基本編譯選項
CFLAGS = -g -std=c11 $(WARNINGS)

# 優化選項（發布版本使用）
RELEASE_FLAGS = -O2

# 調試選項
DEBUG_FLAGS = -O0 -DDEBUG

# 目標執行檔
GAME_TARGET = twisted_fables
TEST_TARGET = test

# 源文件
COMMON_SOURCES = vector.c game_init.c game_logic.c game_state.c card_system.c \
                card_combination.c character_system.c debug_log.c utils.c
GAME_SOURCES = mainloop.c $(COMMON_SOURCES)
TEST_SOURCES = test_main.c test_system.c $(COMMON_SOURCES)

# 目標文件
GAME_OBJECTS = $(GAME_SOURCES:.c=.o)
TEST_OBJECTS = $(TEST_SOURCES:.c=.o)

# 標頭檔依賴
DEPS = architecture.h game_init.h game_logic.h game_state.h card_system.h \
       card_combination.h character_system.h debug_log.h utils.h vector.h

# 默認目標
all: $(GAME_TARGET) $(TEST_TARGET)

# 遊戲執行檔
$(GAME_TARGET): $(GAME_OBJECTS)
	$(CC) $(CFLAGS) -o $@ $^

# 測試執行檔
$(TEST_TARGET): $(TEST_OBJECTS)
	$(CC) $(CFLAGS) -o $@ $^

# 編譯規則
%.o: %.c $(DEPS)
	$(CC) $(CFLAGS) -c $<

# 清理
clean:
	rm -f *.o twisted_fables test test.log

# 運行測試
test: $(TEST_TARGET)
	.\$(TEST_TARGET)

# 運行遊戲
run: $(GAME_TARGET)
	.\$(GAME_TARGET)

# 幫助訊息
help:
	@echo 可用的命令：
	@echo make all            - 構建遊戲和測試程式
	@echo make clean         - 清理所有生成的檔案
	@echo make test          - 運行測試
	@echo make run           - 運行遊戲
	@echo make game          - 只構建遊戲
	@echo make debug         - 構建調試版本
	@echo make release       - 構建優化的發布版本
	@echo make check-warnings - 檢查代碼中的警告
	@echo make show-flags    - 顯示使用的編譯選項

# 僅構建遊戲
game: $(GAME_TARGET)

# 調試版本
debug: CFLAGS += $(DEBUG_FLAGS)
debug: clean all

# 發布版本
release: CFLAGS += $(RELEASE_FLAGS)
release: clean all

# 只檢查警告
check-warnings:
	$(CC) $(CFLAGS) -fsyntax-only $(GAME_SOURCES)

# 顯示編譯訊息
show-flags:
	@echo "使用的編譯選項："
	@echo "CFLAGS = $(CFLAGS)"

.PHONY: all clean test run help game debug
