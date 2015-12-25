#ifndef ROOM_H
#define ROOM_H

#include "action.h"

class Room {
public:
    std::string desc;
    std::map<std::string, Room *> exits;
    std::map<std::string, Action *> actions;
    std::vector<std::unique_ptr<Item>> items;
    std::vector<std::unique_ptr<Creature>> creatures;
    void display() const;
};

#endif

