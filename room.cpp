#include <iostream>
#include <map>
#include <memory>
#include <vector>

#include <ncurses.h>

#include "creature.h"
#include "room.h"

void Room::display() const
{
    printw("%s\n", desc.c_str());

    // Display creatures in the room.
    for (std::vector<std::unique_ptr<Creature>>::const_iterator creature = creatures.begin();
         creature != creatures.end(); ++creature)
    {
        attron(COLOR_PAIR(1));
        printw("%s\n", (*creature)->name.c_str());
        attroff(COLOR_PAIR(1));
    }

    // Display items in the room.
    for (std::vector<std::unique_ptr<Item>>::const_iterator item = items.begin();
         item != items.end(); ++item)
    {
        attron(COLOR_PAIR(2));
        printw("%s\n", (*item)->name.c_str());
        attroff(COLOR_PAIR(2));
    }

    // Display exits from the room.
    printw("Exits: ");

    auto exit = exits.begin();

    if (exit == exits.end())
    {
        printw("None");
    } else
    {
        for (; exit != exits.end(); ++exit)
        {
            printw("%s", exit->first.c_str());
            if (std::distance(exit, exits.end()) > 1)
            {
                printw(", ");
            }
        }
    }
    printw("\n");
}
