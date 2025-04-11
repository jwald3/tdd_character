#include <cmath>
#include <functional>
#include <iostream>
#include <map>
#include <sstream>
#include <stdexcept>
#include <string>

#include "CombatSystem.h"
#include "StatusEffect.h"

Character::Character() {}
Character::Character(std::string name, int health)
    : name{name}, maxHealth{health}, currentHealth{health} {}

Character Character::createWarrior(const std::string& name) {
    Character warrior = Character(name, 100);
    warrior.setStat("Strength", 16);
    warrior.addToInventory("Longsword");

    warrior.equip("Longsword", "Weapon");

    return warrior;
}

Character Character::createMage(const std::string& name) {
    Character mage = Character(name, 100);
    mage.setStat("Intelligence", 16);
    mage.addToInventory("Staff");

    mage.equip("Staff", "Weapon");

    return mage;
}

Character Character::createRogue(const std::string& name) {
    Character rogue = Character(name, 100);
    rogue.setStat("Dexterity", 16);
    rogue.addToInventory("Dagger");

    rogue.equip("Dagger", "Weapon");

    return rogue;
}

void Character::setName(std::string value) { name = value; }

std::string Character::getName() { return name; }

// level
int Character::getLevel() { return level; }

void Character::gainExperience(int exp) {
    int totalExperience = experience += exp;

    level = totalExperience / 100 + 1;
    experience = totalExperience - ((level - 1) * 100);

    maxHealth = (level - 1) * 10 + 100;
}

int Character::getExperience() { return experience; }

// hp system
void Character::setHealth(int value) { maxHealth = value; }

int Character::getHealth() { return currentHealth; }

int Character::getMaxHealth() { return maxHealth; }

void Character::takeDamage(int value) {
    currentHealth = std::max(0, currentHealth - value);
}
void Character::heal(int value) {
    if (currentHealth + value > maxHealth) {
        currentHealth = maxHealth;
    } else {
        currentHealth += value;
    }
}

bool Character::isDead() { return currentHealth == 0; }

// stats
void Character::setStat(std::string stat, int value) { stats[stat] = value; }

int Character::getStat(std::string stat) { return stats[stat]; }

// items
void Character::equip(std::string item, std::string slot) {
    if (inventory.find(item) == inventory.end()) {
        throw std::domain_error("cannot equip items not in inventory");
    }

    gear[slot] = item;
}

std::string Character::getEquipped(std::string slot) { return gear[slot]; }

void Character::addToInventory(std::string item, int count) {
    // check if item already exists
    if (inventory.count(item) != 0) {
        inventory[item] += count;
    } else {
        inventory[item] = count;
    }
}

bool Character::hasItem(std::string item) { return inventory.count(item) > 0; }

int Character::getItemCount(std::string item) { return inventory[item]; }

int Character::getInventoryCount() { return static_cast<int>(inventory.size()); }

bool Character::useItem(std::string item, int count) {
    if (inventory.count(item) != 0 && inventory[item] >= count) {
        inventory[item] -= count;
        return true;
    }

    return false;
}

// combat
void Character::attack(Character& character) {
    // damage = character.stats.strength + weapon.damage
    int weaponDamage = weaponDamageLookup[gear["Weapon"]];
    int damage = stats["Strength"] + weaponDamage;

    int targetNumber = 100 - int(critSettings.rate * 100);
    int rolled = (rand() % 100) + 1;

    if (rolled > targetNumber) {
        int modifiedDamage = (int)(damage * critSettings.modifier);
        character.takeDamage(modifiedDamage);
    } else {
        character.takeDamage(damage);
    }
}

void Character::setWeaponDamage(std::string weapon, int damage) {
    weaponDamageLookup[weapon] = damage;
}

void Character::setCriticalRate(double critChance) { critSettings.rate = critChance; }

void Character::setCriticalMultiplier(double damageMultiplier) {
    critSettings.modifier = damageMultiplier;
}

// abilities
void Character::learnAbility(std::string ability,
                  std::function<bool(Character&, Character&)> abilityFunction) {
    abilityLookup[ability] = abilityFunction;
}

bool Character::useAbility(std::string ability, Character& target) {
    if (abilityLookup.find(ability) == abilityLookup.end()) {
        return false;
    }

    return abilityLookup[ability](*this, target);
}

// status effects
void Character::applyStatusEffect(std::string status, int turnCount) {
    statusEffects[status] = turnCount;
}

bool Character::hasStatusEffect(std::string status) {
    return statusEffects.count(status) != 0;
}

// turns
void Character::processTurn() {
    // iterate over each status effect, do the associated lambda, then take off
    // a turn
    for (auto it = statusEffects.begin(); it != statusEffects.end(); ++it) {
        // look up the status effect
        StatusEffectManager::getStatusEffects()[it->first](*this);

        statusEffects[it->first] -= 1;

        if (it->second == 0) {
            statusEffects.erase(it->first);
        }
    }
}

// serialization
std::string Character::serialize() const {
    std::stringstream ss;
    ss << name << '\n';
    ss << level << '\n';
    ss << experience << '\n';

    ss << stats.size() << '\n';

    for (const auto& pair : stats) {
        ss << pair.first << '\n';
        ss << pair.second << '\n';
    }

    ss << inventory.size() << '\n';

    for (const auto& pair : inventory) {
        ss << pair.first << '\n';
        ss << pair.second << '\n';
    }

    ss << gear.size() << '\n';

    for (const auto& pair : gear) {
        ss << pair.first << '\n';
        ss << pair.second << '\n';
    }

    return ss.str();
}

Character Character::deserialize(const std::string& data) {
    std::stringstream ss(data);
    Character ch{};

    std::getline(ss, ch.name);

    ss >> ch.level;
    ss.ignore();

    ss >> ch.experience;
    ss.ignore();

    size_t statsMapSize;
    ss >> statsMapSize;
    ss.ignore();

    for (size_t i = 0; i < statsMapSize; i++) {
        std::string key;
        int value;

        std::getline(ss, key);
        ss >> value;
        ss.ignore();

        ch.stats[key] = value;
    }

    size_t inventoryMapSize;
    ss >> inventoryMapSize;
    ss.ignore();

    for (size_t i = 0; i < inventoryMapSize; i++) {
        std::string key;
        int value;

        std::getline(ss, key);
        ss >> value;
        ss.ignore();

        ch.inventory[key] = value;
    }

    size_t gearMapSize;
    ss >> gearMapSize;
    ss.ignore();

    for (size_t i = 0; i < gearMapSize; i++) {
        std::string key;
        std::string value;

        std::getline(ss, key);
        std::getline(ss, value);
        ss.ignore();

        ch.gear[key] = value;
    }

    return ch;
}
