#ifndef CREATURE_H
#define CREATURE_H

#include "item.h"

struct Creature final {
    std::string name;
    std::string desc;
    std::vector<std::unique_ptr<Item>> items;
};

#endif
