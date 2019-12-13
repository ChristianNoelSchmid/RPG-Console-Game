/* 
 * Christian Schmid
 * CS202 - Programming Systems
 * Assignment 1 - magician.h
 * Definition of Ranger class, which is a 
 * Character, and focuses on dexterity and archery
 */

#ifndef RANGER_H_
#define RANGER_H_

#include "character.h"

// Default Ranger Stats - used in Constructors
const Stats RAN_STATS = Stats(10, 14, 8, 6); 

// Default Ranger WeaponList - used in Constructors
const WeaponList RAN_WEAPS = WeaponList(
    new Weapon*[3] {
        new Bow(),
        new Dualswords(),
        new Dagger()
    },
    3
);

// Default Ranger PowerList - used in Constructors
const PowerList RAN_POWERS = PowerList(
    new Power*[1] {
        new Sharpshoot("Sharpshoot", 12,
            "Shoot your opponent in the legs, knocking them prone.",
            "You breath in deeply, draw back your shot, and fire.",
            "You hit! Your opponent falls to the ground, cursing!",
            "You miss!")
    },
    1
);

class Ranger : public Character
{
    public:
        Ranger();
        Ranger(const Ranger & from);
        Ranger(const char *name);
        Ranger(const char *name, int exp);

        Character *clone() const override;

        void print(const bool name_only) const override;

    private:
};

#endif