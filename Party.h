#pragma once

#include <string>
#include <map>
#include "character.h"

class Party {
private:
    std::string partyName {};
    std::map<std::string, Character> partyMembers {};

public:
    Party(std::string name);
    void addMember(Character c);
    int getMemberCount();
    bool hasMember(std::string memberName);
    void removeMember(std::string memberName);
};