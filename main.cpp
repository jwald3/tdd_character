#include "CharacterTests.h"

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

    TestRunner::runTest("StatusEffects", testStatusEffects);
    TestRunner::runTest("CombatModifiers", testCombatModifiers);
    TestRunner::runTest("StackableInventory", testStackableInventory);

    TestRunner::runTest("PartyMechanics", testPartyMechanics);
    TestRunner::runTest("CharacterSerialization", testCharacterSerialization);

    TestRunner::runTest("CompleteBattleScenario", testCompleteBattleScenario);

    return 0;
}