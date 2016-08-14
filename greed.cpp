#include <chrono>
#include <iostream>
#include <iterator>
#include <map>
#include <memory>
#include <stdexcept>
#include <vector>

#include <ncurses.h>

#include "item.h"
#include "creature.h"
#include "room.h"
#include "output.h"
#include "capsule.h"

namespace {
    namespace Local {
        int tick_duration = 2;
    }
}

static std::vector<std::unique_ptr<Room>> create_area()
{
    std::vector<std::unique_ptr<Room>> area;

    // This is a special room. Regardless of what you type here,
    // it'll be considered a valid action. When you've performed
    // a certain number of actions, you'll leave the room.
    auto capsule = std::make_unique<Capsule>();
    capsule->desc = "It's dark. You try to blink, but nothing happens.";

    auto medical_bay1 = std::make_unique<Room>();
    medical_bay1->desc = "You find yourself in the medical bay of a large vessel.";

    capsule->exits["unnamed"] = medical_bay1.get();

    auto zombie = std::make_unique<Creature>();
    zombie->name = "A zombie.";

    medical_bay1->creatures.push_back(std::move(zombie));

    auto medical_bay2 = std::make_unique<Room>();
    medical_bay2->desc = "You find yourself in the medical bay of a large vessel.\n"
                         "The wast hallway continues to the north of here.";

    auto search = std::make_unique<Output>("You search the room and notice some scratches on the walls.\n");
    medical_bay2->actions["search"] = std::move(search);

    medical_bay1->exits["n"] = medical_bay2.get();
    medical_bay2->exits["s"] = medical_bay1.get();

    // Add rooms to area.
    area.push_back(std::move(capsule));
    area.push_back(std::move(medical_bay1));
    area.push_back(std::move(medical_bay2));

    return area;
}

static void init_ncurses()
{
    initscr();

    // When we reach the bottom of the screen, the existing text
    // should scroll upwards, just like on a real terminal.
    scrollok(stdscr, TRUE);

    // Enable non-blocking input.
    nodelay(stdscr, TRUE);

    start_color();
    init_pair(1, COLOR_RED, COLOR_BLACK);
    init_pair(2, COLOR_YELLOW, COLOR_BLACK);
}

int main()
{
    std::string kbd_input;
    auto area = create_area();
    auto current_room = area.front().get();

    init_ncurses();

    current_room->enter();
    printw("> ");

    auto start = std::chrono::steady_clock::now();
    std::chrono::seconds tick(Local::tick_duration);

    while (true)
    {
        // Handle all timed events, such as combat.
        if ((std::chrono::steady_clock::now() - start) > tick)
        {
            start = std::chrono::steady_clock::now();
            // printw("Tick! Tock!\n");
        }

        int input_char;

        // Handle user input without blocking.
        if ((input_char = getch()) != ERR)
        {
            if (input_char == '\n')
            {
                bool valid_input = false;
                //const auto& previous_room = current_room;
                const auto previous_room = current_room;

                // Execute action.
                current_room = current_room->perform_action(kbd_input, valid_input);

                // Enter room.
                current_room = current_room->next_room(kbd_input, valid_input);

                if (previous_room != current_room)
                {
                    current_room->enter();
                }

                // The input was neither a valid exit or action.
                if (!valid_input)
                {
                    printw("What?\n");
                }

                printw("> ");
                kbd_input.clear();
            }
            else
            {
                kbd_input.push_back(input_char);
            }
        }
    }

    endwin();
}
