#include "Party.h"

Party::Party(std::string name) : partyName{name} {}

void Party::addMember(Character c) {
    partyMembers.insert({ c.getName(), c });
}
int Party::getMemberCount() {
    return static_cast<int>(partyMembers.size());
}

bool Party::hasMember(std::string memberName) {
    return partyMembers.count(memberName) != 0;
}

void Party::removeMember(std::string memberName) {
    partyMembers.erase(memberName);
}