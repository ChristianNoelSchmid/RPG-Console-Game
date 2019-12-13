/*
 * Christian Schmid
 * CS202 - Programming Systems
 * Program 1 - powerlist.h
 */

#ifndef POWERLIST_H_
#define POWERLIST_H_

#include "power.h"

// Due to circular dependencies, Power is forward declared here
class Power;

class PowerList
{
    public:
        PowerList();
        PowerList(const PowerList & source);
        PowerList(Power **pwrs, int count);
        ~PowerList();

        // Retrieves Power at specified index, if index
        // falls within bounds. Returns NULL if it doesn't
        const Power* get(int index) const;

        // Returns # of elements in the PowerList
        int get_count() const;

    private:
        Power **data;
        int count;
};
#endif