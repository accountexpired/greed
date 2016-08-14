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

    auto tussy = std::make_unique<Creature>();
    tussy->name = "A zombie named Tussy.";

    medical_bay1->creatures.push_back(std::move(tussy));

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

/*
static std::vector<Room *> create_area()
{
    // TODO: Generate areas from an external source, e.g. an SQLite DB-file.
    std::vector<Room *> area;

    auto arrival = new Room();
    arrival->desc = "It's dark. You try to blink, but it makes no difference.";

    auto move = new Output("You try to move, but you bump your nose into some "
                           "glass that\n prevents you from moving any further.\n");

    arrival->actions["north"] = move;
    arrival->actions["south"] = move;
    arrival->actions["east"] = move;
    arrival->actions["west"] = move;

    auto search = new Output("You search around. Your arms feel numb. "
                             "You find nothing.\n");

    arrival->actions["search"] = search;

    auto say = new Output("You emit a sound that resembles a wounded animal.\n"
                          "No eligible word comes out.\n");

    arrival->actions["say"] = say;

    auto knock = new Output("You manage to knock on the glass despite the lack "
                            "of space. Nothing happens.\n");

    arrival->actions["knock"] = knock;

    auto help = new Output(
        "You cry for help, but the voice you hear sounds strange and rough.\n"
        "After several attemps, you utter the word.\n"
        "\n"
        "You hear a beeping sound, followed by a click.\n"
        "\n"
        "The door in front of you changes and fresh air rushes to your face.\n"
        "Relieved, you fall on your knees and cough several times.\n");

    arrival->actions["help"] = help;
    arrival->actions["scream"] = help;
    arrival->actions["yell"] = help;

    // Add rooms to the area.
    area.push_back(arrival);

    return area;
}
*/

/*
static std::vector<Room *> create_area_2()
{
    // TODO: Generate areas from an external source, e.g. an SQLite DB-file.
    std::vector<Room *> area;

    auto sword = std::make_unique<Item>();
    sword->name = "A golden sword.";

    auto boat = std::make_unique<Item>();
    boat->name = "A boat.";

    auto arrival = new Room();
    arrival->desc = "At a mountain summit. The wind is howling.";
    arrival->items.push_back(std::move(sword));
    arrival->items.push_back(std::move(boat));

    auto blink = new Output("You blink, and a snake slithers by!\n");

    auto wolf = std::make_unique<Creature>();
    wolf->name = "A fierce wolf.";

    auto tussy = std::make_unique<Creature>();
    tussy->name = "It's Tusselita.";

    auto forest = new Room();
    forest->desc = "Lots of trees here.";
    forest->creatures.push_back(std::move(wolf));
    forest->creatures.push_back(std::move(tussy));
    forest->actions["blink"] = blink;

    // Set room exits.
    area.push_back(arrival);
    forest->exits["up"] = arrival;
    area.push_back(forest);
    arrival->exits["down"] = forest;

    return area;
}
*/
