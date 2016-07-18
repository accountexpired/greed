#ifndef CAPSULE_H
#define CAPSULE_H

#include "room.h"

class Capsule final : public Room
{
public:
    Room* perform_action(const std::string& kbd_input,
                         bool& valid_input) override;
private:
    int action_attempts = 1;
};

#endif
