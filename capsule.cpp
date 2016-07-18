#include <iostream>
#include <ncurses.h>

#include "capsule.h"

namespace
{
namespace Local
{
    const int min_attempts = 5;
}
}

Room* Capsule::perform_action(const std::string& kbd_input, bool& valid_input)
{
    (void)kbd_input; // Silence warning.
    valid_input = true;

    switch (action_attempts)
    {
    case 1: printw("You are stuck and can't move!\n");
        ++action_attempts;
        break;
    case 2: printw("Your body doesn't respond!\n");
        ++action_attempts;
        break;
    case 3: printw("Nothing happens.\n");
        ++action_attempts;
        break;
    case 4: printw("You begin to panic!\n");
        ++action_attempts;
        break;
    case 5: printw("You break through the glass and gasp for air!\n\n");
        return exits.at("unnamed");
    }

    return this;
}
