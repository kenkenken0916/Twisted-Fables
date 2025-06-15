CC = gcc
CFLAGS = -std=c11 -Wall -Wextra -g -O2
TARGET = twisted_fables

# Source files  
SOURCES = src/core/game_init.c src/core/game_logic.c src/core/game_state.c src/core/mainloop.c \
          src/systems/card_system.c src/systems/character_system.c \
          src/utils/debug_log.c src/utils/utils.c src/utils/vector.c \
          src/main.c

OBJECTS = $(SOURCES:.c=.o)

# Main target
$(TARGET): $(OBJECTS)
	$(CC) $(OBJECTS) -o $(TARGET)

# Object files
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Clean
clean:
	rm -f $(OBJECTS) $(TARGET)

# Phony targets
.PHONY: clean
