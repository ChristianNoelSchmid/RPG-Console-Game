/* 
 * Christian Schmid
 * CS202 - Programming Systems
 * Assignment 1 - magician.h
 * Definition of Warrior class, which is a 
 * Character, and focuses on melee combat
 */

#ifndef WARRIOR_H_
#define WARRIOR_H_

#include "character.h"

// Default Warrior Stats - used in Constructors
const Stats WARRIOR_STATS = Stats(14, 10, 8, 8);

// Default Warrior WeaponList - used in Constructors
const WeaponList WARRIOR_WEAPS = WeaponList (
    new Weapon*[2] {
        new Greataxe(),
        new Dagger(),
    },
    2
);

// Default Warrior PowerList - used in Constructors
const PowerList WARRIOR_POWERS = PowerList (
    new Power*[1] {
        new Warcry(
            "Warcry",
            4,
            "Shout, causing lowered ability in your opponent.",
            "You emit a gutteral roar.",
            "Your opponent's knees knock together!",
            "Your opponent laughs at your attempt to scare them."
        )
    },
    1
);

class Warrior : public Character
{
    public:
        Warrior();
        Warrior(const Warrior & source);
        Warrior(const char *name);
        Warrior(const char *name, int exp);

        Character *clone() const override;

        void print(bool name_only) const override;
};

#endif