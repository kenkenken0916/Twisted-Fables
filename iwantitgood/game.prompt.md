# Twisted Fables (扭曲寓言) Game Specification

## Game Overview
Twisted Fables is a 2 or 4-player DBG (Deck Building Game) card battle game featuring dark reimaginings of fairy tale characters. Players control unique characters with special abilities, fighting to reduce their opponent's life points to zero.

## Core Game Components

### Board and Setup
- 1 Game Board with battle tracks
- Character standees/models for positioning
- Life, Defense, Energy, and Special tokens

### Card Types
1. **Basic Cards**
   - Attack Cards (54 cards, 18 each of Levels 1-3)
   - Defense Cards (54 cards, 18 each of Levels 1-3)
   - Movement Cards (54 cards, 18 each of Levels 1-3)
   - Generic Cards (18 cards)

2. **Character-Specific Cards**
   - Each character has 27 cards (Skills, Ultimate, Metamorphosis)
   - Special cards for specific characters
     - Match Girl: 12 Match cards
     - Snow White: 18 Poison cards

### Character Tokens
- Sleeping Beauty: 9 Awakening tokens
- Mulan: 12 Ki tokens
- Little Mermaid: 4 Tentacle tokens
- Dorothy: 12 Combo tokens
- Scheherazade: 6 Destiny tokens

## Game Setup

1. **Character Selection**
   - Each player chooses a character
   - Set initial life and ultimate threshold based on character stats
   - Reset defense and energy to 0

2. **Initial Positioning**
   - Place players at starting positions on battle track
   - In 2v2 mode, teammates cannot be on same track

3. **Deck Setup**
   - Initialize basic supply decks
   - Set up skill supply decks
   - Prepare ultimate cards
   - Create initial deck (12 cards):
     - 3 Level-1 skills
     - 3 Level-1 attack cards
     - 3 Level-1 defense cards
     - 3 Level-1 movement cards

4. **Hand Setup**
   - First player draws 4 cards
   - Second player draws 6 cards

## Turn Structure

### 1. Start Phase
- Resolve ongoing effects

### 2. Cleanup Phase
- Discard cards in play area
- Reset defense value to 0

### 3. Action Phase
Players can perform multiple actions (except Focus):
- **Focus** (Once per turn)
  - Skip all other actions
  - Remove one card from hand or discard pile
  
- **Attack**
  - Use any number of attack cards
  - Deal damage = sum of attack values
  - Range 1 attack
  - Gain energy equal to attack value

- **Defense**
  - Use any number of defense cards
  - Gain defense = sum of defense values
  - Gain energy equal to defense value
  - Cannot exceed max defense

- **Movement**
  - Use any number of movement cards
  - Move X spaces in any direction
  - Gain energy equal to movement value
  - Cannot overlap with other players
  - Can move through opponents

- **Skill Use** (Once per turn)
  - Use 1 skill card + required basic card
  - Effect based on basic card value
  - No energy gain from basic card

- **Ultimate**
  - Use ultimate card when below threshold
  - Powerful unique effects

- **Buy Card**
  - Spend energy to buy from supply
  - Card goes to discard pile
  - Can buy basic or skill cards

### 4. End Phase
- Reset energy to 0
- Discard hand
- Discard cards in play (except ongoing effects)
- Draw 6 cards
- Resolve end-of-turn effects

## Combat Rules
- Damage first reduces defense, then life
- Direct life loss bypasses defense
- Maximum energy is 25
- Healing cannot exceed max life
- No negative values allowed
- Players can check all discard piles
- Players can see number of cards in hands

## Special Card Rules
- Generic cards count as Level 1 of any basic card type
- Ultimate cards available when life ≤ threshold
- Metamorphosis cards provide permanent passive effects
- Cards with ongoing effects remain in play

## 2v2 Mode Rules
- Teams of 2 players
- Clockwise turns alternating teams
- If one player defeated, teammate takes double turns
- Can switch battle tracks using movement cards
- Cannot attack between tracks
- Can share strategy but not hand information

# Card Details

## Red Riding Hood (小紅帽) - ID: 0

### Basic Stats
- Life: 30
- Defense: 6
- Ultimate Threshold: 15

### Skill Usage Rule
Skills must be played with a basic card. The 'O' value in skill effects represents the value of the basic card used:
- If using with Level 1 basic card: O = 1
- If using with Level 2 basic card: O = 2
- If using with Level 3 basic card: O = 3
- If using with Universal card: O = 1

### Attack Skills
1. Quick Shot (快速射擊) - ID: 11
   - Cost: -
   - Range: 1
   - Required: Attack card
   - Effect: Damage = 1 + O
   Example:
   - With Level 1 Attack (O=1): Damage = 2
   - With Level 2 Attack (O=2): Damage = 3
   - With Level 3 Attack (O=3): Damage = 4

2. Precision Shot (精準射擊) - ID: 12
   - Cost: 2
   - Range: 2
   - Required: Attack card
   - Effect: Damage = 2 + O
   Example:
   - With Level 1 Attack (O=1): Damage = 3
   - With Level 2 Attack (O=2): Damage = 4
   - With Level 3 Attack (O=3): Damage = 5

3. Lethal Shot (致命狙擊) - ID: 13
   - Cost: 4
   - Range: 3
   - Required: Attack card
   - Effect: Damage = 3 + O
   Example:
   - With Level 1 Attack (O=1): Damage = 4
   - With Level 2 Attack (O=2): Damage = 5
   - With Level 3 Attack (O=3): Damage = 6

### Defense Skills
1. Energy Shield (能量護盾) - ID: 14
   - Cost: -
   - Required: Defense card
   - Effect: 
     - Range: 1
     - Damage: 1
     - Defense: O (equal to used defense card value)
     - Continuous: At start of next turn, if defense > 0, deal 2 damage to opponent within range 1
   Example:
   - With Level 1 Defense (O=1): Gain 1 defense
   - With Level 2 Defense (O=2): Gain 2 defense
   - With Level 3 Defense (O=3): Gain 3 defense

2. Current Shield (電流護盾) - ID: 15
   - Cost: 2
   - Required: Defense card
   - Effect:
     - Range: 2
     - Damage: 2
     - Defense: O (equal to used defense card value)
     - Continuous: At start of next turn, if defense > 0, deal 4 damage to opponent within range 2
   Example:
   - With Level 1 Defense (O=1): Gain 1 defense
   - With Level 2 Defense (O=2): Gain 2 defense
   - With Level 3 Defense (O=3): Gain 3 defense

3. Ultimate Shield (終極護盾) - ID: 16
   - Cost: 4
   - Required: Defense card
   - Effect:
     - Range: 3
     - Damage: 3
     - Defense: O (equal to used defense card value)
     - Continuous: At start of next turn, if defense > 0, deal 6 damage to opponent within range 3
   Example:
   - With Level 1 Defense (O=1): Gain 1 defense
   - With Level 2 Defense (O=2): Gain 2 defense
   - With Level 3 Defense (O=3): Gain 3 defense

### Movement Skills
1. Ballistic Jet (彈道噴射) - ID: 17
   - Cost: -
   - Range: 1
   - Required: Movement card
   - Effect:
     - Damage: 1
     - Knock back opponent up to O spaces (O = movement card value)
   Example:
   - With Level 1 Movement (O=1): Knockback 1 space
   - With Level 2 Movement (O=2): Knockback 2 spaces
   - With Level 3 Movement (O=3): Knockback 3 spaces

2. Power Jet (火力噴射) - ID: 18
   - Cost: 2
   - Range: 2
   - Required: Movement card
   - Effect:
     - Damage: 2
     - Knock back opponent up to O spaces (O = movement card value)
   Example:
   - With Level 1 Movement (O=1): Knockback 1 space
   - With Level 2 Movement (O=2): Knockback 2 spaces
   - With Level 3 Movement (O=3): Knockback 3 spaces

3. Rage Jet (暴怒噴射) - ID: 19
   - Cost: 4
   - Range: 3
   - Required: Movement card
   - Effect:
     - Damage: 3
     - Knock back opponent up to O spaces (O = movement card value)
   Example:
   - With Level 1 Movement (O=1): Knockback 1 space
   - With Level 2 Movement (O=2): Knockback 2 spaces
   - With Level 3 Movement (O=3): Knockback 3 spaces

### Ultimate Cards
1. Wolf's Devour (餓狼吞噬) - ID: 20
   - Effect: Add top 2 cards from any skill supply deck to hand (not counted as purchase)

2. System Invasion (系統入侵) - ID: 21
   - Effect: Repeat your last action or attack/movement skill used this turn (including metamorphosis effects)

3. Revenge Rain (復仇之雨) - ID: 22
   - Range: 3
   - Effect:
     - Damage 3
     - Knock back opponent up to 3 spaces
     - Opponent must discard 3 cards

### Metamorphosis Cards
1. Overload Burn (過載燃燒) - ID: 135
   - Effect: When using movement or attack skill, can discard up to 1 skill card to gain damage +X (X = discarded card level)

2. Hood System (兜帽系統) - ID: 136
   - Effect: When opponent deals damage, can discard up to 1 skill card to reduce damage by X (X = discarded card level)

3. Mutation Sense (變異感應) - ID: 137
   - Effect: When using movement or attack skill, can discard up to 1 skill card to gain Range +X (X = discarded card level)

4. Board Buffer (板載緩存) - ID: 138
   - Effect:
     - Can store 1 card on this card during action phase
     - Can retrieve stored card during action phase
     - Card not discarded to discard pile

### Special Rules
- In 2v2 mode, when an opponent is defeated, if Board Buffer contains their cards, remove those cards from game

# Character Specifications

## Basic Stats Overview

| Character      | Life | Defense | Ultimate |
|---------------|------|---------|----------|
| Red Riding Hood| 30   | 6       | 15       |
| Snow White    | 34   | 6       | 17       |
| Sleeping Beauty| 42   | 6       | 21       |
| Alice         | 32   | 6       | 16       |
| Mulan         | 34   | 3       | 17       |
| Princess Kaguya| 32   | 6       | 16       |
| Little Mermaid| 36   | 3       | 18       |
| Match Girl    | 36   | 6       | 18       |
| Dorothy       | 40   | 6       | 20       |
| Scheherazade  | 36   | 6       | 18       |

## Character Specifics

### Red Riding Hood
- Focus: Technology-enhanced abilities
- Special Feature: Load Buffer system
- Abilities:
  - Attack cards for ranged damage
  - Defense cards with counter-damage
  - Movement cards with knockback

### Snow White
- Focus: Poison mechanics
- Special Feature: Poison deck (18 cards)
- Abilities:
  - Attack cards with deck manipulation
  - Defense cards that distribute poison
  - Movement cards with positioning

### Sleeping Beauty
- Focus: Awakening mechanism
- Special Feature: Awakening tokens (max 6)
- States:
  - Sleeping: Gains tokens from damage
  - Awakened: Can spend tokens for effects

### Alice
- Focus: Identity switching
- Special Feature: Three identities
  - Queen of Hearts (Attack+1, Move-1)
  - Mad Hatter (Defense+1, Attack-1)
  - Cheshire Cat (Move+1, Defense-1)
- Must switch identity each turn

### Mulan
- Focus: Ki resource management
- Special Feature: Ki tokens
- Abilities:
  - Gain Ki from reducing damage
  - Spend Ki for various effects
  - Position-based advantages

### Princess Kaguya
- Focus: Defense mechanics
- Special Feature: Defense value interactions
- Abilities:
  - Attack bonuses from defense
  - Card manipulation through defense
  - Position-based control

### Little Mermaid
- Focus: Tentacle control
- Special Feature: Tentacle tokens
- Abilities:
  - Area control
  - Position-based damage
  - Tentacle movement tactics

### Match Girl
- Focus: Match card distribution
- Special Feature: Match cards
- Abilities:
  - Energy manipulation
  - Card conversion
  - Match card effects

### Dorothy
- Focus: Combo system
- Special Feature: Combo tokens
- Abilities:
  - Chain skills
  - Position control
  - Multiple ultimates

### Scheherazade
- Focus: Supply deck control
- Special Feature: Destiny tokens
- Abilities:
  - Deck manipulation
  - Resource control
  - Token management

# Card Identification System

## Basic Cards (IDs 1-10)
- Attack Cards
  - Level 1 Attack (ID: 1)
  - Level 2 Attack (ID: 2)
  - Level 3 Attack (ID: 3)
- Defense Cards
  - Level 1 Defense (ID: 4)
  - Level 2 Defense (ID: 5)
  - Level 3 Defense (ID: 6)
- Movement Cards
  - Level 1 Movement (ID: 7)
  - Level 2 Movement (ID: 8)
  - Level 3 Movement (ID: 9)
- Universal Card (ID: 10)

## Character Skill Cards

### Red Riding Hood (Character ID: 0)
- Attack Skills:
  - Quick Shot (ID: 11)
  - Precision Shot (ID: 12)
  - Lethal Shot (ID: 13)
- Defense Skills:
  - Energy Shield (ID: 14)
  - Current Shield (ID: 15)
  - Ultimate Shield (ID: 16)
- Movement Skills:
  - Ballistic Jet (ID: 17)
  - Power Jet (ID: 18)
  - Rage Jet (ID: 19)
- Ultimate Cards:
  - Wolf's Devour (ID: 20)
  - System Invasion (ID: 21)
  - Revenge Rain (ID: 22)

### Snow White (Character ID: 1)
- Attack Skills:
  - Crystal Shard (ID: 23)
  - Crystal Vortex (ID: 24)
  - Crystal Storm (ID: 25)
- Defense Skills:
  - Corrupted Blessing (ID: 26)
  - Corrupted Banquet (ID: 27)
  - Corrupted Revelry (ID: 28)
- Movement Skills:
  - Broken Fantasy (ID: 29)
  - Broken Reality (ID: 30)
  - Broken Destiny (ID: 31)
- Ultimate Cards:
  - Seven Snake Rage (ID: 32)
  - Mirror Rain (ID: 33)
  - Brewing Disaster (ID: 34)
- Special Cards:
  - Poison Level 1 (ID: 131)
  - Poison Level 2 (ID: 132)
  - Poison Level 3 (ID: 133)

### Sleeping Beauty (Character ID: 2)
- Attack Skills:
  - Mind Tremor (ID: 35)
  - Mind Rage (ID: 36)
  - Mind Fury (ID: 37)
- Defense Skills:
  - Burst Lock (ID: 38)
  - Burst Bone (ID: 39)
  - Burst Soul (ID: 40)
- Movement Skills:
  - Dark Touch (ID: 41)
  - Dark Entangle (ID: 42)
  - Dark Strangle (ID: 43)
- Ultimate Cards:
  - Awaken Sleep (ID: 44)
  - Daydream Nightmare (ID: 45)
  - Blood Recast (ID: 46)

### Alice (Character ID: 3)
- Attack Skills:
  - Open Game (ID: 47)
  - Turn Game (ID: 48)
  - Control Game (ID: 49)
- Defense Skills:
  - Magic Trick (ID: 50)
  - Mental Illusion (ID: 51)
  - Hat Trick (ID: 52)
- Movement Skills:
  - Strange Agility (ID: 53)
  - Strange Stealth (ID: 54)
  - Strange Strangeness (ID: 55)
- Ultimate Cards:
  - Endless Party (ID: 56)
  - Wonderful Wonder Day (ID: 57)
  - Game Under Control (ID: 58)

### Mulan (Character ID: 4)
- Attack Skills:
  - Not to be Underestimated (ID: 59)
  - Unstoppable (ID: 60)
  - Indestructible (ID: 61)
- Defense Skills:
  - Still Controls Moving (ID: 62)
  - Soft Overcomes Hard (ID: 63)
  - Weak Defeats Strong (ID: 64)
- Movement Skills:
  - Never Retreat (ID: 65)
  - No Mercy (ID: 66)
  - No Forgiveness (ID: 67)
- Ultimate Cards:
  - Sky-High Ki (ID: 68)
  - Face Chaos (ID: 69)
  - Thunder Strike (ID: 70)

### Princess Kaguya (Character ID: 5)
- Attack Skills:
  - Light of Understanding (ID: 71)
  - Glory of Understanding (ID: 72)
  - Avatar of Understanding (ID: 73)
- Defense Skills:
  - Echo of Confusion (ID: 74)
  - Echo of Eternity (ID: 75)
  - Divine Summon (ID: 76)
- Movement Skills:
  - Focus Introspection (ID: 77)
  - Enlightened Resolution (ID: 78)
  - Painful Purification (ID: 79)
- Ultimate Cards:
  - Hot Bamboo Sword (ID: 80)
  - Destined Judgment (ID: 81)
  - Restless Blood (ID: 82)

### Little Mermaid (Character ID: 6)
- Attack Skills:
  - Siren's Call (ID: 83)
  - Siren's Song (ID: 84)
  - Siren's Screech (ID: 85)
- Defense Skills:
  - Surging Rage (ID: 86)
  - Gushing Rage (ID: 87)
  - Vengeful Rage (ID: 88)
- Movement Skills:
  - Abyss Consume (ID: 89)
  - Abyss Invade (ID: 90)
  - Abyss Conquer (ID: 91)
- Ultimate Cards:
  - Mermaid Revival (ID: 92)
  - Ancient Awakening (ID: 93)
  - Purifying Tide (ID: 94)

### Match Girl (Character ID: 7)
- Attack Skills:
  - Illusory Wish (ID: 95)
  - Secret Hope (ID: 96)
  - Insatiable Desire (ID: 97)
- Defense Skills:
  - Devil's Offering (ID: 98)
  - Devil's Gamble (ID: 99)
  - Devil's Contract (ID: 100)
- Movement Skills:
  - Weightless Soul (ID: 101)
  - Indebted Soul (ID: 102)
  - Broken Soul (ID: 103)
- Ultimate Cards:
  - Hell Flame (ID: 104)
  - Doom Descend (ID: 105)
  - Greed Curse (ID: 106)
- Special Cards:
  - Match Card (ID: 134)

### Dorothy (Character ID: 8)
- Attack Skills:
  - Target Confirm (ID: 107)
  - Target Lock (ID: 108)
  - Target Clear (ID: 109)
- Defense Skills:
  - Mind Probe (ID: 110)
  - Deep Search (ID: 111)
  - Read Complete (ID: 112)
- Movement Skills:
  - Enemy Track (ID: 113)
  - Enter Vision (ID: 114)
  - Mission End (ID: 115)
- Ultimate Cards:
  - Lion (ID: 116)
  - Tin Man (ID: 117)
  - Scarecrow (ID: 118)

### Scheherazade (Character ID: 9)
- Attack Skills:
  - Erase Dream (ID: 119)
  - Destroy Memory (ID: 120)
  - Annihilate Existence (ID: 121)
- Defense Skills:
  - Submerge Net (ID: 122)
  - Obsession Net (ID: 123)
  - Dissolve Net (ID: 124)
- Movement Skills:
  - Watch Eye (ID: 125)
  - Control Hand (ID: 126)
  - Dominate Mind (ID: 127)
- Ultimate Cards:
  - System Delete (ID: 128)
  - No Escape (ID: 129)
  - Cut Connection (ID: 130)

## Metamorphosis Cards (IDs 135-176)

### Red Riding Hood
- Overload Burn (ID: 135)
- Hood System (ID: 136)
- Mutation Sense (ID: 137)
- Board Buffer (ID: 138)

### Snow White
- Crystal Coffin (ID: 139)
- Fall Ritual (ID: 140)
- Toxic Erosion (ID: 141)
- Pure Poison (ID: 142)

### Sleeping Beauty
- Blood Therapy (ID: 143)
- Blood Sacrifice (ID: 144)
- Spirit Barrier (ID: 145)
- Forced Treatment (ID: 146)

### Alice
- Off with Her Head (ID: 147)
- Wonderland Descend (ID: 148)
- We're All Mad Here (ID: 149)
- Start My Show (ID: 150)

### Mulan
- Ki Through Body (ID: 151)
- Destiny Master (ID: 152)
- Long March (ID: 153)
- Storm Coming (ID: 154)

### Princess Kaguya
- Punishment Time (ID: 155)
- Blood Moon (ID: 156)
- Spirit Instinct (ID: 157)
- Moonlight Meditation (ID: 158)

### Little Mermaid
- Storm Erosion (ID: 159)
- Mystic Resonance (ID: 160)
- Daughter of Sea (ID: 161)
- Dark Tide Rising (ID: 162)

### Match Girl
- Painful Ritual (ID: 163)
- Indulgent Desire (ID: 164)
- Devil's Gaze (ID: 165)
- Flame's Trick (ID: 166)
- Desire's Trick (ID: 167)
- Fate's Trick (ID: 168)

### Dorothy
- Kill Command (ID: 169)
- Beyond Machine (ID: 170)
- Execute Extreme (ID: 171)
- Nowhere to Hide (ID: 172)

### Scheherazade
- Hand of Fate (ID: 173)
- Rewrite Desire (ID: 174)
- Reconstruct Thought (ID: 175)
- Tale Weaver (ID: 176)

## Implementation Notes

### Key Systems
1. **Card System**
   - Card ID ranges specified in card_num_spec.md
   - Value calculation and type checking
   - Effect resolution
   - Deck and hand management

2. **Character System**
   - Unique abilities and passives
   - Special resource management
   - Skill combinations

3. **Game State Management**
   - Turn tracking
   - Action validation
   - Effect resolution
   - State transitions

4. **Combat System**
   - Damage calculation
   - Defense handling
   - Range checking
   - Position tracking

### Technical Architecture
1. **Core System Files**
   - `architecture.h`: Core system architecture
     - Player structure (`struct _player`)
     - Game state enums (`enum state`)
     - Vector types
   - `mainloop.c`: Main game loop and initialization

2. **Game Logic Modules**
   - `game_logic.c/h`: Core game mechanics
     - Player choice handling
     - State transitions
     - Game end conditions
   - `game_init.c/h`: Game initialization
   - `game_state.c/h`: State management

3. **Card System**
   - `card_system.c/h`: Card core functionality
     - Card initialization
     - Value calculation
     - Requirement checking
     - Cost calculation
   - `card_combination.c/h`: Card combinations

4. **Character System**
   - `character_system.c/h`: Character implementation
     - Character initialization
     - Skill handling
     - Passive effects
     - Character-specific functions

5. **Support Systems**
   - `vector.c/h`: Dynamic array implementation
   - `debug_log.c/h`: Debug logging system
   - `utils.c/h`: Utility functions

### Development Guidelines

1. **Coding Standards**
   - Follow C11 standard
   - Enable strict warnings
   - Maintain code clarity

2. **Error Management**
   - Implement thorough error checks
   - Use DEBUG_LOG for operations
   - Document error conditions

3. **Memory Management**
   - Track dynamic allocations
   - Prevent memory leaks
   - Use vector structures

4. **Testing Requirements**
   - Add tests for new features
   - Verify all changes
   - Run complete test suite

5. **Version Control**
   - Use git
   - Ensure compilation
   - Clear commit messages

### Error Handling Specifications

1. **Input Validation Errors**
   - Invalid card combinations
     - Check card types match skill requirements
     - Validate level requirements
     - Verify energy costs
     - Return error code and message to UI
   
   - Invalid actions
     - Check action availability in current phase
     - Verify resource costs (energy, tokens, etc.)
     - Validate target selection (range, visibility)
     - Return descriptive error message

   - Invalid state transitions
     - Verify phase transition requirements
     - Check game state consistency
     - Validate turn order
     - Log state transition failures

2. **Resource Management Errors**
   - Memory allocation failures
     - Validate all dynamic allocations
     - Implement cleanup on allocation failure
     - Maintain system stability
     - Log allocation failures

   - Card deck errors
     - Handle empty deck conditions
     - Validate deck shuffling
     - Check deck size constraints
     - Log deck manipulation errors

   - Token management errors
     - Validate token counts
     - Check token placement rules
     - Handle token overflow/underflow
     - Track token state changes

3. **Network and Synchronization Errors**
   - Connection failures
     - Implement timeout handling
     - Store game state for recovery
     - Handle disconnection gracefully
     - Log connection events

   - State synchronization errors
     - Validate game state consistency
     - Handle out-of-sync conditions
     - Implement state recovery
     - Log synchronization issues

4. **Game Logic Errors**
   - Combat resolution errors
     - Validate damage calculations
     - Check range requirements
     - Verify effect applications
     - Log combat resolution issues

   - Special ability errors
     - Validate ability requirements
     - Check resource availability
     - Handle ability interactions
     - Track ability usage

5. **Recovery Procedures**
   - Game state recovery
     ```c
     bool recover_game_state(game* gameState) {
         // Log recovery attempt
         DEBUG_LOG("Attempting game state recovery");
         
         // Validate current state
         if (!validate_game_state(gameState)) {
             ERROR_LOG("Invalid game state detected");
             return false;
         }
         
         // Restore last valid state
         if (!restore_last_valid_state(gameState)) {
             ERROR_LOG("State restoration failed");
             return false;
         }
         
         // Verify recovery
         if (!verify_state_consistency(gameState)) {
             ERROR_LOG("Recovery verification failed");
             return false;
         }
         
         DEBUG_LOG("Game state recovery successful");
         return true;
     }
     ```

   - Resource cleanup
     ```c
     void cleanup_resources(game* gameState) {
         // Log cleanup start
         DEBUG_LOG("Starting resource cleanup");
         
         // Free dynamic allocations
         free_card_resources(gameState);
         free_token_resources(gameState);
         free_player_resources(gameState);
         
         // Reset state
         reset_game_state(gameState);
         
         DEBUG_LOG("Resource cleanup completed");
     }
     ```

6. **Error Logging and Reporting**
   - Error severity levels
     ```c
     typedef enum {
         LOG_DEBUG,    // Development information
         LOG_INFO,     // Normal operations
         LOG_WARNING,  // Potential issues
         LOG_ERROR,    // Recoverable errors
         LOG_FATAL     // Unrecoverable errors
     } LogLevel;
     ```

   - Logging format
     ```c
     void log_error(LogLevel level, const char* module, const char* message) {
         // Get current timestamp
         time_t now = time(NULL);
         char timestamp[26];
         ctime_r(&now, timestamp);
         timestamp[24] = '\0';  // Remove newline
         
         // Log with format: [TIME][LEVEL][MODULE] Message
         fprintf(stderr, "[%s][%s][%s] %s\n",
                 timestamp,
                 log_level_str(level),
                 module,
                 message);
     }
     ```

7. **Error Prevention Guidelines**
   - Pre-condition validation
     - Check input parameters
     - Validate state requirements
     - Verify resource availability
   
   - Post-condition validation
     - Verify operation results
     - Check state consistency
     - Validate resource cleanup
   
   - Invariant maintenance
     - Track game rules compliance
     - Monitor resource limits
     - Verify state transitions

Each error handling procedure should:
1. Log the error with appropriate severity
2. Attempt recovery if possible
3. Clean up resources if necessary
4. Return meaningful error codes/messages
5. Maintain system stability

### Extension Points

1. **Adding Characters**
   - Define in `character_system.h`
   - Implement special abilities
   - Create test cases
   - Balance considerations

2. **Adding Cards**
   - Define in `card_system.h`
   - Implement effects
   - Update combinations
   - Test interactions

3. **New Game Modes**
   - Add to `game_logic.c`
   - Consider balance
   - Ensure compatibility
   - Test thoroughly
