/* 
 * Christian Schmid
 * CS202 - Programming Systems
 * Assignment 1 - magician.h
 * Definition of Magician class, which is a 
 * Character, and focuses on magic casting
 */

#ifndef MAGICIAN_H_
#define MAGICIAN_H_

#include "character.h"

// Default Magician Stats - used in Constructors
const Stats MAG_STATS = Stats(8, 10, 14, 2);

// Default Magician WeaponList - used in Constructors
const WeaponList MAG_WEAPS = WeaponList(
    new Weapon*[2] {
        new Staff(),
        new Dagger(),
    },
    2
);

// Default Magician Powers - used in Constructors
const PowerList MAG_POWERS = PowerList(
    new Power*[2] {
        new Fire("Fireball", 20, 
            "Hurl fire at your opponent, dealing damage to health and armor.",
            "You muster up a flame, shooting it at your adversary.",
            "You hit!", "You miss! (You managed to singe their hair though, so you damage their style)."),
        new Heal("Heal Self", 20,
            "Cure your wounds, burns, cuts, and other ailments.",
            "You flourish a little twirl of green, and place your hand upon your chest.",
            "You have healed", "")
    },
    2
);

class Magician : public Character
{
    public:
        Magician();
        Magician(const Magician & from);
        Magician(const char *name);
        Magician(const char *name, int exp);

        void print(const bool name_only) const override;
        Character *clone() const override;

    private:
};

#endif