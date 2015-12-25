#include <iostream>
#include <ncurses.h>

#include "output.h"

Output::Output()
{
}

Output::~Output()
{
    std::cout << "Destroying blink!" << std::endl;
}

Output::Output(std::string newOutput) : message(newOutput)
{
}

void Output::exec() const
{
    printw(message.c_str());
}

