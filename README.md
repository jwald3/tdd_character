# RPG Character System

A flexible character system for RPG games, built using test-driven development (TDD). The system implements core RPG mechanics including combat, inventory management, character stats, abilities, and party management.

## Features

### Character Management
- Create characters with different classes (Warrior, Mage, Rogue)
- Level up system with experience points
- Health and damage system
- Customizable stats (Strength, Intelligence, Dexterity, etc.)

### Combat System
- Basic attack mechanics
- Weapon damage modifiers
- Critical hit system with configurable rates and multipliers
- Special abilities system
- Status effects (e.g., Poison)
- Turn-based combat processing

### Inventory System
- Item management with stackable items
- Equipment system with different slots (Weapon, Armor, etc.)
- Item usage mechanics

### Party System
- Create parties with multiple characters
- Add/remove party members
- Track party composition

### Additional Features
- Serialization support for save/load functionality
- Extensive test coverage
- Modular design for easy extension

## Development Approach

This project was developed using Test-Driven Development (TDD), where tests were written first to define the desired functionality, followed by implementation to satisfy those tests. This approach helped ensure:

- Clear feature specifications
- High test coverage
- Robust implementation
- Clean, maintainable code

## Project Structure

- `character.h/cpp` - Core character class implementation
- `Party.h/cpp` - Party management system
- `CombatSystem.h` - Combat-related structures and mechanics
- `StatusEffect.h` - Status effect management
- `CharacterTests.h/cpp` - Comprehensive test suite
- `TestRunner.h/cpp` - Test execution framework

## Tests

The test suite covers:
- Character creation and basic attributes
- Combat mechanics
- Inventory management
- Party system
- Status effects
- Experience and leveling
- Equipment system
- Serialization
- Complete battle scenarios

## Building and Running

1. Ensure you have a C++ compiler installed
2. Clone the repository
3. Build the project using your preferred build system
4. Run the tests to verify functionality

## Future Enhancements

Potential areas for expansion:
- Additional character classes
- More complex combat mechanics
- Extended status effect system
- Quest system integration
- Save/load game state
- Network multiplayer support
