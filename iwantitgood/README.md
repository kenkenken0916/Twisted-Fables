# Twisted Fables (扭曲寓言) - Deck Building Game

A 2 or 4-player DBG (Deck Building Game) card battle game featuring dark reimaginings of fairy tale characters.

## Game Overview

Players control unique characters with special abilities, fighting to reduce their opponent's life points to zero through strategic card play and positioning.

## Features

- 10 unique characters with distinct abilities
- Complex card combination system
- Turn-based combat with positioning
- Special tokens and resource management
- 2v2 team mode support

## Building and Running

```bash
make clean
make
./twisted_fables
```

## Project Structure

```
src/
├── core/
│   ├── architecture.h      # Core system definitions
│   ├── mainloop.c         # Main game loop
│   ├── game_logic.c/h     # Core game mechanics
│   ├── game_init.c/h      # Game initialization
│   └── game_state.c/h     # State management
├── systems/
│   ├── card_system.c/h    # Card functionality
│   ├── character_system.c/h # Character implementation
│   └── vector.c/h         # Dynamic arrays
├── utils/
│   ├── debug_log.c/h      # Debug logging
│   └── utils.c/h          # Utility functions
└── tests/
    └── test_*.c           # Unit tests
```

## Character List

1. Red Riding Hood - Technology-enhanced ranged combat
2. Snow White - Poison mechanics and deck manipulation
3. Sleeping Beauty - Awakening tokens and state changes
4. Alice - Identity switching with stat modifiers
5. Mulan - Ki resource management
6. Princess Kaguya - Defense-based abilities
7. Little Mermaid - Tentacle positioning control
8. Match Girl - Match card distribution system
9. Dorothy - Combo token chaining
10. Scheherazade - Supply deck manipulation

## Game Rules

- Turn-based gameplay with multiple action phases
- Cards require basic cards + skill cards for activation
- Combat involves range, damage, and defense mechanics
- Special tokens provide unique character abilities
- Victory condition: Reduce opponent's life to 0

## Development

This project follows C11 standards with strict error checking and memory management.
