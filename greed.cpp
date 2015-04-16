#include <iostream>
#include <iterator>
#include <map>
#include <memory>
#include <stdexcept>
#include <vector>
#include <unordered_set>

std::string color(std::string hue)
{
    static const std::map<std::string, int> hues = {
        {"default",  0},
        {"black",   30},
        {"red",     31},
        {"green",   32},
        {"yellow",  33},
        {"blue",    34},
        {"magenta", 35},
        {"cyan",    36},
        {"white",   37}
    };

    std::map<std::string, int>::const_iterator i = hues.find(hue);

    if (i == hues.end()){
        return "";
    } else if (i->first == "default") {
        return "\033[0m";
    }

    return "\033[;" + std::to_string(hues.find(hue)->second) + "m";
}

class Item {
public:
    std::string name;
    std::string desc;
};

class Creature {
public:
    std::string name;
    std::string desc;
    std::vector<Item*> items;
};

class Room {
public:
    std::string desc;
    std::map<std::string, Room*> exits;
    std::vector<Item*> items;
    std::vector<Creature*> creatures;
    void display() const;
};

void Room::display() const
{
    std::cout << desc << std::endl;

    // Display creatures in the room.
    for (std::vector<Creature*>::const_iterator creature = creatures.begin();
         creature != creatures.end(); ++creature) {
        std::cout << color("red") << (*creature)->name << color("default") << std::endl;
    }

    // Display items in the room.
    for (std::vector<Item*>::const_iterator item = items.begin(); item != items.end(); ++item) {
        std::cout << color("yellow") << (*item)->name << color("default") << std::endl;
    }

    // Display exits from the room.
    std::cout << "Exits: ";
    for (std::map<std::string, Room*>::const_iterator exit = exits.begin();
         exit != exits.end(); ++exit) {
        std::cout << exit->first;
        if (std::distance(exit, exits.end()) > 1)
        {
            std::cout << ", ";
        }
    }
    std::cout << "." << std::endl;
}

int main(void)
{
    std::unordered_set<std::string> dirs = {"n", "s", "e", "w"};

    Item sword;
    sword.name = "A golden sword.";

    Item boat;
    boat.name = "A boat.";

    Creature wolf;
    wolf.name = "A fierce wolf.";

    Creature tussy;
    tussy.name = "It's Tusselita.";

    Room arrival;
    arrival.desc = "At a mountain summit. The wind is howling.";
    arrival.items.push_back(&sword);
    arrival.items.push_back(&boat);

    Room forest;
    forest.desc = "Lots of trees here.";
    forest.creatures.push_back(&wolf);

    Room forest2;
    forest2.desc = "You have reached the end of the forest.";
    forest2.creatures.push_back(&tussy);

    arrival.exits["n"] = &forest;
    forest.exits["s"] = &arrival;
    forest.exits["e"] = &forest2;
    forest2.exits["w"] = &forest;

    Room* currentRoom = &arrival;
    std::string kbdInput;

    currentRoom->display();
    std::cout << "> ";

    while (std::cin >> kbdInput) {
        if (dirs.find(kbdInput) != dirs.end()) {
            // Received valid direction. Can you go there?
            if (currentRoom->exits.find(kbdInput) == currentRoom->exits.end()) {
                std::cout << "You can not go there!" << std::endl;
            } else {
                currentRoom = currentRoom->exits[kbdInput];
                currentRoom->display();
            }
        } else {
            std::cout << "What?" << std::endl;
        }
        std::cout << "> ";
    }

    return 0;
}
