#include <cmath>
#include <string>
#include <map>
#include <stdexcept>
#include <iostream>
#include <functional>

struct CriticalHitSettings {
    double rate;
    double modifier;
};

class Character {
   private:
    std::string name{};
    int maxHealth{};
    int currentHealth{};
    int experience {};
    int level { 1 };

    // stat : value
    std::map<std::string, int> stats {};
    // item : 1
    std::map<std::string, int> inventory {};
    // slot : item
    std::map<std::string, std::string> gear {};
    // weapon name : damage
    std::map<std::string, int> weaponDamageLookup {};
    // ability : effect
    std::map<std::string, std::function<bool(Character&, Character&)>> abilityLookup {};
    // status : turns
    std::map<std::string, int> statusEffects {}; 

    CriticalHitSettings critSettings {};



    // status : effect
    std::map<std::string, std::function<void(Character&)>> statusLookup = {
        { "Poison", [](Character& character) {
            character.takeDamage(5);
        }}
    };

   public:
    Character(std::string name, int health)
        : name{name}, maxHealth{health}, currentHealth{health} {};

    static Character createWarrior(const std::string& name) {
        Character warrior = Character(name, 100);
        warrior.setStat("Strength", 16);
        warrior.addToInventory("Longsword");

        warrior.equip("Longsword", "Weapon");

        return warrior;
    }

    static Character createMage(const std::string& name) {
        Character mage = Character(name, 100);
        mage.setStat("Intelligence", 16);
        mage.addToInventory("Staff");


        mage.equip("Staff", "Weapon");

        return mage;
    }

    static Character createRogue(const std::string& name) {
        Character rogue = Character(name, 100);
        rogue.setStat("Dexterity", 16);
        rogue.addToInventory("Dagger");

        rogue.equip("Dagger", "Weapon");

        return rogue;
    }

    void setName(std::string value) { name = value; }

    std::string getName() { return name; }

    // level
    int getLevel() {
        return level;
    }

    void gainExperience(int exp) {
        int totalExperience = experience += exp;

        level = totalExperience / 100 + 1;
        experience = totalExperience - ((level - 1) * 100);

        maxHealth = (level - 1) * 10 + 100; 
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
        if (inventory.find(item) == inventory.end()) {
            throw std::domain_error("cannot equip items not in inventory");
        }

        gear[slot] = item;
    }

    std::string getEquipped(std::string slot) {
        return gear[slot];
    }

    void addToInventory(std::string item, int count = 1) {
        // check if item already exists
        if (inventory.count(item) != 0) {
            inventory[item] += count;
        } else {
            inventory[item] = count;
        }
    }

    bool hasItem(std::string item) {
        return inventory.count(item) > 0;
    }

    int getItemCount(std::string item) {
        return inventory[item];
    }

    int getInventoryCount() {
        return static_cast<int>(inventory.size());
    }

    bool useItem(std::string item, int count) {
        if (inventory.count(item) != 0 && inventory[item] >=  count) {
            inventory[item] -= count;
            return true;
        }

        return false;
    }

    // combat
    void attack(Character& character) {
        // damage = character.stats.strength + weapon.damage
        int weaponDamage = weaponDamageLookup[gear["Weapon"]];
        int damage = stats["Strength"] + weaponDamage;

        int targetNumber = 100 - int (critSettings.rate * 100);
        int rolled = (rand() % 100) + 1;

        if (rolled > targetNumber) {
            int modifiedDamage = (int) (damage * critSettings.modifier);
            character.takeDamage(modifiedDamage);
        } else {
            character.takeDamage(damage);
        }
    }

    void setWeaponDamage(std::string weapon, int damage) {
        weaponDamageLookup[weapon] = damage;
    }

    void setCriticalRate(double critChance) {
        critSettings.rate = critChance;
    }

    void setCriticalMultiplier(double damageMultiplier) {
        critSettings.modifier = damageMultiplier;
    }

    // abilities
    void learnAbility(std::string ability, std::function<bool(Character&, Character&)> abilityFunction) {
        abilityLookup[ability] = abilityFunction;
    }

    bool useAbility(std::string ability, Character& target) {
        if (abilityLookup.find(ability) == abilityLookup.end()) {
            return false;
        }

        return abilityLookup[ability](*this, target);
    }

    // status effects
    void applyStatusEffect(std::string status, int turnCount) {
        statusEffects[status] = turnCount;
    }

    bool hasStatusEffect(std::string status) {
        return statusEffects.count(status) != 0;
    }

    // turns
    void processTurn() {
        // iterate over each status effect, do the associated lambda, then take off a turn
        for (auto it = statusEffects.begin(); it != statusEffects.end(); ++it) {
            // look up the status effect
            statusLookup[it->first](*this);

            statusEffects[it->first] -= 1;

            if (it->second == 0) {
                statusEffects.erase(it->first);
            }
        }
    }

    //
};

