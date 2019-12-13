/*
 * Christian Schmid
 * CS202 - Programming Systems
 * Program 1 - stats.h
 * 
 * Definition of Stats - a class that holds
 * all information related to a Character's ability
 * makeup.
 */

#ifndef STATS_H_
#define STATS_H_

#include "abilitybase.h"
#include "functions.h"

class Stats
{
    public:
        // Default constructor
        Stats(); // Constructor takes three arguments, which represent
                 // the three base abilities: strength, deterity, and will
        Stats(const int strength, const int dexter, const int will, const int armr);

        // Adds experience to Stats, adjusting all
        // abilities and max's in the process. Also resets
        // health, stamina, and magic
        void add_exp(int to_add);

        // Restores health by specified amount
        void restore_health(int value);

        // Restores stamina back to max amount
        void restore_stamina();

        // Subtracts specified amount from ability
        void decrement_ability(const AbilityBase base, const int value);

        // Retrieves the modifier for a particular Ability, which can
        // be used for attacks, defenses, dodges, etc.
        int get_abl_mod(const AbilityBase base) const;

        // Subtract health, blocked by armor, unless ign_armor is true
        int take_damage(const int value, const bool ign_armor);
        void take_armor_damage(const int value);

        // Returns Stats experience value
        int get_exp_value() const;

    protected:
        // Prints the Stats into the console. Requires derived class to
        // call
        void print() const;
   
        // adjusted by experience
        int get_str() const;
        int get_dex() const;
        int get_will() const;

        // Max health, stamina, and magic,
        // found by strength, dexterity, and will
        int get_max_health() const;
        int get_max_stam() const;
        int get_max_mag() const;

        // Stats experience - mathamatically adjusts base_str,
        // base_dex, and base_will
        int exp;

        // Fight stats
        int health, stamina, magic;

        // Armor stats
        int armor;

    private:
        // Base abilities: strength, dexterity, will
        // get_str(), get_dex(), and get_will() are
        // determined by these values (+experience)
        int base_str, base_dex, base_will;
};

#endif
