/*
 * Christian Schmid
 * CS202 - Programming Systems
 * Program 1 - character.h
 * 
 * Definition of Character class - inherits from Stats and is
 * the base class for all class types (ie. Magician)
 * Provides Battle functionality and stores all BattleHistory
 * information
 * 
 */
#ifndef CHARACTER_H_
#define CHARACTER_H_

#include<cstring>
#include<fstream>

#include "battlehistory.h"
#include "powerlist.h"
#include "stats.h"
#include "weapon.h"

// Due to circular dependencies, BattleHistory, Power, 
// PowerList, Weapon, and WeaponList are forward declared here
class BattleHistoryList;

class Power;
class PowerList;

class Weapon;
class WeaponList;

// Enum of all CharacterTypes - used to distinguish
// between Character derivations in data structures
enum CharacterType
{
    MAGICIAN,
    RANGER,
    WARRIOR
};

class Character : public Stats
{
    public:
        // Constructors / Destructors
        Character();
        Character(const Character & copy_from);
        Character(const CharacterType char_type, const char *nme, 
            const Stats stats, const WeaponList & weapon_list,
            const PowerList & power_list
        );
        virtual ~Character();

        // Clones the Character, maintaining derived class
        // identity
        virtual Character *clone() const = 0;

        // Compares two Character classes, checking if they
        // are equal
        virtual bool equals(const Character & other) const;
        // Prints the Character - either their full Stats
        // or only their name
        virtual void print(bool name_only) const;

        // Provides the CharacterType - used in ADT CharacterList implementations
        virtual CharacterType character_type() const;

        // The primary console sequence a Character follows when it's
        // their turn in combat
        virtual void prompt_action(Character & character);

        // Casts a power the Character has, against supplied Character opponent
        virtual bool cast_power(int index, Character & opponent);

        // Attempts to dodge attack 
        virtual bool attempt_dodge(const AbilityBase base, const int roll_value) const;

        // Knock the Character prone
        virtual void knock_prone();

        // Determines if Character's hp is below 0
        virtual bool is_dead() const;

        // Prints the Character's list of BattleHistories
        virtual void print_history() const;

        // Inserts a won Battle into the Character's BattleHistory
        virtual void insert_win(const Character & opponent, int exp);

        // Inserts a lost Battle into the Character's BattleHistory
        virtual void insert_loss(const Character & opponent, int ext);

        // Takes an ifstream and copies data into BattleHistory lists,
        // seperating wins and losses
        virtual void load_battle_history(std::ifstream & ifs);

        // Appends Character data to the provided ofstream
        virtual void save(std::ofstream & ofs);

    protected:
        CharacterType char_type; // The type of Character(Magician, Ranger, etc.)
        char *name; // The Character's name
        bool is_prone; // Is the Character prone?
        WeaponList *weapon_list; // The Character's list of weapons
        PowerList *power_list; // The Character's list of Powers
        BattleHistoryList *win_list; // The Character's BattleHistoryList of wins
        BattleHistoryList *loss_list; // The Character's BattleHistoryList of losses
};

/* ********************************
 */

#endif
