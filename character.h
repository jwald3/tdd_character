#include <cmath>
#include <string>
#include <map>
#include <stdexcept>
#include <iostream>
#include <functional>

class Character {
   private:
    std::string name{};
    int maxHealth{};
    int currentHealth{};
    int experience {};
    int level {};

    // stat : value
    std::map<std::string, int> stats {};
    // item : 1
    std::map<std::string, int> inventory {};
    // slot : item
    std::map<std::string, std::string> gear {};
    // weapon name : damage
    std::map<std::string, int> weaponDamageLookup {};

   public:
    Character(std::string name, int health)
        : name{name}, maxHealth{health}, currentHealth{health} {};

    static Character createWarrior(const std::string& name) {
        return Character(name, 100);
    }

    static Character createMage(const std::string& name) {
        return Character(name, 100);
    }

    static Character createRogue(const std::string& name) {
        return Character(name, 100);
    }

    void setName(std::string value) { name = value; }

    std::string getName() { return name; }

    // level
    int getLevel() {
        return level;
    }

    void gainExperience(int exp) {
        experience += exp;
    }

    int getExperience() {
        return experience;
    }

    // hp system
    void setHealth(int value) { maxHealth = value; }

    int getHealth() { return currentHealth; }

    int getMaxHealth() { return maxHealth; }

    void takeDamage(int value) {
        currentHealth = std::max(0, currentHealth - value);
    }
    void heal(int value) {
        if (currentHealth + value > maxHealth) {
            currentHealth = maxHealth;
        } else {
            currentHealth += value;
        }
    }

    bool isDead() {
        return currentHealth == 0;
    }

    // stats
    void setStat(std::string stat, int value) {
        stats[stat] = value;
    }

    int getStat(std::string stat) {
        return stats[stat];
    }

    // items
    void equip(std::string item, std::string slot) {
        if (inventory.count(item) == 0) throw std::domain_error("cannot equip items not in inventory");

        gear[slot] = item;
    }

    std::string getEquipped(std::string slot) {
        return gear[slot];
    }

    void addToInventory(std::string item) {
        inventory[item] = 1;
    }

    bool hasItem(std::string item) {
        return inventory.count(item) > 0;
    }

    int getInventoryCount() {
        return static_cast<int>(inventory.size());
    }

    // combat
    void attack(Character& character) {
        // damage = character.stats.strength + weapon.damage
        int weaponDamage = weaponDamageLookup[gear["Weapon"]];
        int damage = stats["Strength"] + weaponDamage;

        character.takeDamage(damage);
    }

    void setWeaponDamage(std::string weapon, int damage) {
        weaponDamageLookup[weapon] = damage;
    }

    // abilities
    void learnAbility(std::string ability, std::function<bool(Character&, Character&)> abilityFunction) {

    }

    bool useAbility(std::string ability, Character& target) {
        return true;
    }
};