#include "character.h"

#include <functional>
#include <iostream>
#include <string>

class TestRunner {
   public:
    static void runTest(const std::string& testName,
                        std::function<bool()> test) {
        std::cout << "Running test: " << testName << "... ";
        if (test()) {
            std::cout << "PASSED" << std::endl;
        } else {
            std::cout << "FAILED" << std::endl;
        }
    }
};

#define ASSERT_EQ(expected, actual)                                           \
    ((expected) == (actual)) ? true                                           \
                             : (std::cout << "Assertion failed: " << expected \
                                          << " != " << actual << std::endl,   \
                                false)

// Fixed test function with correct return type and logic
bool testCreateCharacterWithNameAndHealth() {
    Character character("Adventurer", 100);

    bool nameCorrect =
        ASSERT_EQ(std::string("Adventurer"), character.getName());
    bool healthCorrect = ASSERT_EQ(100, character.getHealth());

    return nameCorrect && healthCorrect;
}

bool testCharacterTakesDamage() {
    Character character("Warrior", 100);

    character.takeDamage(30);
    bool healthAfterDamage = ASSERT_EQ(70, character.getHealth());

    character.takeDamage(50);
    bool healthAfterMoreDamage = ASSERT_EQ(20, character.getHealth());

    return healthAfterDamage && healthAfterMoreDamage;
}

bool testHealthCannotBeBelowZero() {
    Character character("Paladin", 50);

    character.takeDamage(70);  // Trying to take more damage than current health
    bool healthAtZero = ASSERT_EQ(0, character.getHealth());

    return healthAtZero;
}

// Test for character healing
bool testCharacterCanHeal() {
    Character character("Cleric", 60);

    character.takeDamage(30);
    bool damagedHealth = ASSERT_EQ(30, character.getHealth());

    character.heal(20);
    bool healedHealth = ASSERT_EQ(50, character.getHealth());

    return damagedHealth && healedHealth;
}

// Test that health cannot exceed maximum health
bool testHealthCannotExceedMaximum() {
    Character character("Mage", 40);

    character.heal(10);
    bool initialHeal =
        ASSERT_EQ(40, character.getHealth());  // Should remain at maximum

    character.takeDamage(15);
    bool damagedHealth = ASSERT_EQ(25, character.getHealth());

    character.heal(30);  // Trying to heal beyond maximum
    bool healedHealth =
        ASSERT_EQ(40, character.getHealth());  // Should only heal to maximum

    return initialHeal && damagedHealth && healedHealth;
}

// Test for character inventory
bool testCharacterCanAddItemToInventory() {
    Character character("Rogue", 60);

    character.addToInventory("Dagger");
    character.addToInventory("Lockpick");

    bool hasDagger = ASSERT_EQ(true, character.hasItem("Dagger"));
    bool hasLockpick = ASSERT_EQ(true, character.hasItem("Lockpick"));
    bool doesNotHaveSword = ASSERT_EQ(false, character.hasItem("Sword"));

    bool correctItemCount = ASSERT_EQ(2, character.getInventoryCount());

    return hasDagger && hasLockpick && doesNotHaveSword && correctItemCount;
}

// Test for character stats/attributes
bool testCharacterHasStats() {
    Character character("Warrior", 100);

    character.setStat("Strength", 18);
    character.setStat("Dexterity", 12);
    character.setStat("Intelligence", 8);

    bool strengthCorrect = ASSERT_EQ(18, character.getStat("Strength"));
    bool dexterityCorrect = ASSERT_EQ(12, character.getStat("Dexterity"));
    bool intelligenceCorrect = ASSERT_EQ(8, character.getStat("Intelligence"));
    bool nonExistentStatIsZero = ASSERT_EQ(0, character.getStat("Charisma"));

    return strengthCorrect && dexterityCorrect && intelligenceCorrect &&
           nonExistentStatIsZero;
}

// Test equipment functionality
bool testCharacterCanEquipItems() {
    Character character("Fighter", 80);

    // Items must be in inventory before equipping
    character.addToInventory("Longsword");
    character.addToInventory("Shield");
    character.addToInventory("Plate Armor");

    character.equip("Longsword", "Weapon");
    character.equip("Shield", "Offhand");
    character.equip("Plate Armor", "Armor");

    bool weaponEquipped =
        ASSERT_EQ("Longsword", character.getEquipped("Weapon"));
    bool offhandEquipped =
        ASSERT_EQ("Shield", character.getEquipped("Offhand"));
    bool armorEquipped =
        ASSERT_EQ("Plate Armor", character.getEquipped("Armor"));
    bool emptySlotIsEmpty = ASSERT_EQ("", character.getEquipped("Helmet"));

    return weaponEquipped && offhandEquipped && armorEquipped &&
           emptySlotIsEmpty;
}

bool testCharacterCanAttackOthers() {
    Character attacker("Berserker", 100);
    Character defender("Guardian", 120);

    attacker.setStat("Strength", 16);

    // Basic attack does strength damage
    attacker.attack(defender);
    bool damageDealt = ASSERT_EQ(104, defender.getHealth());  // 120 - 16

    return damageDealt;
}

// Test for weapon damage modifiers
bool testWeaponDamageModifiers() {
    Character attacker("Swordsman", 80);
    Character defender("Target", 150);

    attacker.setStat("Strength", 15);
    attacker.addToInventory("Steel Sword");
    attacker.setWeaponDamage("Steel Sword", 10);
    attacker.equip("Steel Sword", "Weapon");

    // Attack with weapon should do strength + weapon damage
    attacker.attack(defender);
    bool correctDamage =
        ASSERT_EQ(125, defender.getHealth());  // 150 - (15 + 10)

    return correctDamage;
}

// Test for character death
bool testCharacterDeath() {
    Character attacker("Assassin", 70);
    Character victim("Civilian", 30);

    attacker.setStat("Strength", 35);

    attacker.attack(victim);
    bool victimDied = ASSERT_EQ(true, victim.isDead());
    bool attackerAlive = ASSERT_EQ(false, attacker.isDead());

    return victimDied && attackerAlive;
}

// Test for character experience and leveling
bool testCharacterExperienceAndLeveling() {
    Character character("Adventurer", 100);

    // Characters should start at level 1
    bool startsAtLevelOne = ASSERT_EQ(1, character.getLevel());

    // Gain some experience but not enough to level up
    character.gainExperience(50);
    bool hasCorrectXP = ASSERT_EQ(50, character.getExperience());
    bool stillLevelOne = ASSERT_EQ(1, character.getLevel());

    // Gain enough experience to level up (assume 100 XP per level)
    character.gainExperience(60);
    bool leveledUp = ASSERT_EQ(2, character.getLevel());
    bool correctXPAfterLevel = ASSERT_EQ(
        10,
        character.getExperience());  // 110 total, 100 for level, 10 remaining

    // Verify stats increase with leveling
    bool healthIncreased =
        ASSERT_EQ(110, character.getMaxHealth());  // +10 per level

    return startsAtLevelOne && hasCorrectXP && stillLevelOne && leveledUp &&
           correctXPAfterLevel && healthIncreased;
}

// Test for special abilities
bool testCharacterSpecialAbilities() {
    Character attacker("Wizard", 60);
    Character target("Enemy", 100);

    // Define a special ability
    attacker.learnAbility("Fireball", [](Character& user, Character& target) {
        int intelligence = user.getStat("Intelligence");
        target.takeDamage(intelligence *
                          2);  // Fireball does double intelligence as damage
        return true;
    });

    attacker.setStat("Intelligence", 15);

    // Use the ability
    bool abilityUsed = attacker.useAbility("Fireball", target);
    bool correctDamage = ASSERT_EQ(70, target.getHealth());  // 100 - (15 * 2)

    // Try to use non-existent ability
    bool nonExistentAbilityFails =
        !attacker.useAbility("Lightning Bolt", target);
    bool healthUnchanged =
        ASSERT_EQ(70, target.getHealth());  // Should still be 70

    return abilityUsed && correctDamage && nonExistentAbilityFails &&
           healthUnchanged;
}

// Test for character classes
bool testCharacterClasses() {
    // Creating different classes of characters
    Character warrior = Character::createWarrior("Brutus");
    Character mage = Character::createMage("Merlin");
    Character rogue = Character::createRogue("Shadow");

    // Each class should have appropriate starting stats
    bool warriorStrength = ASSERT_EQ(16, warrior.getStat("Strength"));
    bool mageIntelligence = ASSERT_EQ(16, mage.getStat("Intelligence"));
    bool rogueDexterity = ASSERT_EQ(16, rogue.getStat("Dexterity"));

    // Each class should have appropriate starting equipment
    bool warriorWeapon = ASSERT_EQ("Longsword", warrior.getEquipped("Weapon"));
    bool mageWeapon = ASSERT_EQ("Staff", mage.getEquipped("Weapon"));
    bool rogueWeapon = ASSERT_EQ("Dagger", rogue.getEquipped("Weapon"));

    return warriorStrength && mageIntelligence && rogueDexterity &&
           warriorWeapon && mageWeapon && rogueWeapon;
}

int main() {
    TestRunner::runTest("CreateCharacterWithNameAndHealth",
                        testCreateCharacterWithNameAndHealth);
    TestRunner::runTest("CharacterTakesDamage", testCharacterTakesDamage);
    TestRunner::runTest("HealthCannotBeBelowZero", testHealthCannotBeBelowZero);
    TestRunner::runTest("CharacterCanHeal", testCharacterCanHeal);
    TestRunner::runTest("HealthCannotExceedMaximum",
                        testHealthCannotExceedMaximum);
    TestRunner::runTest("CharacterCanAddItemToInventory",
                        testCharacterCanAddItemToInventory);
    TestRunner::runTest("CharacterHasStats", testCharacterHasStats);
    TestRunner::runTest("CharacterCanEquipItems", testCharacterCanEquipItems);
    TestRunner::runTest("CharacterCanAttackOthers",
                        testCharacterCanAttackOthers);
    TestRunner::runTest("WeaponDamageModifiers", testWeaponDamageModifiers);
    TestRunner::runTest("CharacterDeath", testCharacterDeath);

    TestRunner::runTest("CharacterExperienceAndLeveling",
                        testCharacterExperienceAndLeveling);
    TestRunner::runTest("CharacterSpecialAbilities",
                        testCharacterSpecialAbilities);
    TestRunner::runTest("CharacterClasses", testCharacterClasses);

    return 0;
}