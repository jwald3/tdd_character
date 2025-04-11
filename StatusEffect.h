#pragma once
#include <functional>
#include <map>
#include <string>

#include "character.h"

class Character;

class StatusEffectManager {
   public:
    static std::map<std::string, std::function<void(Character&)>>
    getStatusEffects() {
        return {
            {"Poison", [](Character& character) { character.takeDamage(5); }}};
    }
};