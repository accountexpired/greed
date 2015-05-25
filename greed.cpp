#include <iostream>
#include <iterator>
#include <map>
#include <memory>
#include <stdexcept>
#include <vector>

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
    std::vector<std::unique_ptr<Item>> items;
};

class Room {
public:
    std::string desc;
    std::map<std::string, Room *> exits;
    std::vector<std::unique_ptr<Item>> items;
    std::vector<std::unique_ptr<Creature>> creatures;
    void display() const;
};

void Room::display() const
{
    std::cout << desc << std::endl;

    // Display creatures in the room.
    for (std::vector<std::unique_ptr<Creature>>::const_iterator creature = creatures.begin();
         creature != creatures.end(); ++creature) {
        std::cout << color("red") << (*creature)->name << color("default") << std::endl;
    }

    // Display items in the room.
    for (std::vector<std::unique_ptr<Item>>::const_iterator item = items.begin();
         item != items.end(); ++item) {
        std::cout << color("yellow") << (*item)->name << color("default") << std::endl;
    }

    // Display exits from the room.
    std::cout << "Exits: ";
    for (std::map<std::string, Room *>::const_iterator exit = exits.begin();
         exit != exits.end(); ++exit) {
        std::cout << exit->first;
        if (std::distance(exit, exits.end()) > 1)
        {
            std::cout << ", ";
        }
    }
    std::cout << "." << std::endl;
}

std::vector<Room> create_area()
{
    // TODO: Generate areas from an external source, e.g. an SQLite DB-file.
    std::vector<Room> area(3);

    std::unique_ptr<Item> sword(new Item);
    // std::unique_ptr<Item> sword = std::make_unique<Item>(); // C++14
    sword->name = "A golden sword.";

    std::unique_ptr<Item> boat(new Item);
    boat->name = "A boat.";

    std::unique_ptr<Creature> wolf(new Creature);
    wolf->name = "A fierce wolf.";

    std::unique_ptr<Creature> tussy(new Creature);
    tussy->name = "It's Tusselita.";

    Room& arrival = area[0];
    arrival.desc = "At a mountain summit. The wind is howling.";
    arrival.items.push_back(std::move(sword));
    arrival.items.push_back(std::move(boat));

    Room& forest = area[1];
    forest.desc = "Lots of trees here.";
    forest.creatures.push_back(std::move(wolf));

    Room& forest2 = area[2];
    forest2.desc = "You have reached the end of the forest.";
    forest2.creatures.push_back(std::move(tussy));

    arrival.exits["n"] = &forest;
    forest.exits["s"] = &arrival;
    forest.exits["e"] = &forest2;
    forest2.exits["w"] = &forest;

    return area;
}

int main(void)
{
    std::vector<Room> area = create_area();

    Room *currentRoom = &area[0];
    std::string kbdInput;

    currentRoom->display();
    std::cout << "> ";

    while (std::cin >> kbdInput) {
        // If the input was a direction, go there and switch to the corresponding room.
        if (currentRoom->exits.find(kbdInput) != currentRoom->exits.end()) {
            currentRoom = currentRoom->exits[kbdInput];
            currentRoom->display();
        } else {
            std::cout << "What?" << std::endl;
        }
        std::cout << "> ";
    }

    return 0;
}
