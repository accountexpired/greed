#ifndef ROOM_H
#define ROOM_H

#include <map>
#include <memory>
#include <vector>

#include "creature.h"
#include "item.h"
#include "action.h"

class Room {
private:
    void display() const;

public:
    virtual ~Room();

    std::string desc;
    std::map<std::string, Room *> exits;
    std::map<std::string, std::unique_ptr<Action>> actions;
    std::vector<std::unique_ptr<Item>> items;
    std::vector<std::unique_ptr<Creature>> creatures;

    virtual Room* perform_action(const std::string& kbd_input,
                                 bool& valid_input);
    Room* next_room(const std::string& kbd_input, bool& valid_input);
    void enter() const;
};

#endif
