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

static std::vector<Room *> create_area()
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

int main(void)
{
    std::string kbdInput;
    std::vector<Room *> area = create_area();
    Room *currentRoom = area[0];

    init_ncurses();

    currentRoom->display();
    printw("> ");

    auto start = std::chrono::steady_clock::now();
    std::chrono::seconds tick(2); // Each tick should last two seconds.

    while (true)
    {
        // Handle all timed events, such as combat.
        if ((std::chrono::steady_clock::now() - start) > tick)
        {
            start = std::chrono::steady_clock::now();
            printw("Tick! Tock!\n");
        }

        int inputChar;

        // Handle user input without blocking.
        if ((inputChar = getch()) != ERR)
        {
            if (inputChar == '\n')
            {
                if (currentRoom->exits.find(kbdInput) != currentRoom->exits.end())
                {
                    currentRoom = currentRoom->exits[kbdInput];
                    currentRoom->display();
                }
                else if (currentRoom->actions.find(kbdInput) != currentRoom->actions.end())
                {
                    // Perform the requested action.
                    currentRoom->actions[kbdInput]->exec();
                } else
                {
                    printw("What?\n");
                }

                printw("> ");
                kbdInput.clear();
            } else
            {
            kbdInput.push_back(inputChar);
            }
        }
    }

    // Clean up.
    for (auto& room : area)
    {
        for (auto& action : room->actions)
        {
            delete action.second;
        }
        delete room;
    }

    endwin();

    return 0;
}
