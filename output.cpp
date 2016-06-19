#include <iostream>
#include <ncurses.h>

#include "output.h"

Output::Output()
{
}

Output::~Output()
{
}

Output::Output(std::string new_output) : message(new_output)
{
}

void Output::exec() const
{
    printw(message.c_str());
}

