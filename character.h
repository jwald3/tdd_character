#pragma once
#include <string>
#include <map>
#include <functional>
#include "CombatSystem.h"


class Character{
private:
    std::string name{};
    int maxHealth{};
    int currentHealth{};
    int experience{};
    int level{1};

    std::map<std::string, int> stats {};
    std::map<std::string, int> inventory {};
    std::map<std::string, std::string> gear {};
    std::map<std::string, int> weaponDamageLookup {};
    std::map<std::string, std::function<bool(Character&, Character&)>> abilityLookup {};
    std::map<std::string, int> statusEffects {}; 

    CriticalHitSettings critSettings {};
public: 
    Character();
    Character(std::string name, int health);

    // factory methods
    static Character createWarrior(const std::string& name);
    static Character createMage(const std::string& name);
    static Character createRogue(const std::string& name);

    void setName(std::string value);
    std::string getName();

    // level and exp
    int getLevel();
    void gainExperience(int exp);
    int getExperience();

    // health system
    void setHealth(int value);
    int getHealth();
    int getMaxHealth();
    void takeDamage(int value);
    void heal(int value);
    bool isDead();

    // stats
    void setStat(std::string stat, int value);
    int getStat(std::string stat);

    // items + equipment
    void equip(std::string item, std::string slot);
    std::string getEquipped(std::string slot);
    void addToInventory(std::string item, int count = 1);
    bool hasItem(std::string item);
    int getItemCount(std::string item);
    int getInventoryCount();
    bool useItem(std::string item, int count);

    // combat
    void attack(Character& character);
    void setWeaponDamage(std::string weapon, int damage);
    void setCriticalRate(double critChance);
    void setCriticalMultiplier(double damageMultiplier);

    // abilities
    void learnAbility(std::string ability, std::function<bool(Character&, Character&)> abilityFunction);
    bool useAbility(std::string ability, Character& target);

    // status effects
    void applyStatusEffect(std::string status, int turnCount);
    bool hasStatusEffect(std::string status);
    void processTurn();

    // serialization
    std::string serialize() const;
    static Character deserialize(const std::string& data);
};  