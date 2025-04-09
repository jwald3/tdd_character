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

    return 0;
}