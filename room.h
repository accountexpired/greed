#ifndef ROOM_H
#define ROOM_H

#include "action.h"

class Room final {
private:
    void display() const;

public:
    std::string desc;
    std::map<std::string, Room *> exits;
    std::map<std::string, Action *> actions;
    std::vector<std::unique_ptr<Item>> items;
    std::vector<std::unique_ptr<Creature>> creatures;

    void perform_action(const std::string& kbd_input, bool& valid_input) const;
    Room* next_room(const std::string& kbd_input, bool& valid_input);
    void enter() const;
};

#endif

