#ifndef ACTION_H
#define ACTION_H

class Action {
public:
    virtual ~Action() {};
    virtual void exec() const = 0;
};

#endif

