/*
 * Christian Schmid
 * CS202 - Programming Systems
 * Program 1 - weapon.h
 * Definition of WeaponType enum, and Weapon and
 * WeaponList clases
 * 
 */

#ifndef WEAPON_H_
#define WEAPON_H_

#include<iostream>
#include<cstring>

#include "abilitybase.h"
#include "character.h"
#include "functions.h"

// Include forward declarations of Character and Stats due to
// circular header dependencies
class Character;
class Stats;

// A list of all possible Weapon types - used for ADTs and upcast cloning
enum WeaponType { NA, GREATAXE, DUALSWORDS, STAFF, DAGGER, BOW };

class Weapon
{
    public:
        Weapon();
        Weapon(const Weapon & from);
        Weapon(const char* nme, const WeaponType type,const AbilityBase base, 
               const int min_dmg, const int max_dmg, bool rnged, int prec_bon);
        virtual ~Weapon();

        // Clones the Weapon, returning an upcasted derivation 
        virtual Weapon *clone() const;

        // Displays the Weapon
        virtual void print() const;

        // Attacker attempts to hit the Defender, using Weapon stats
        virtual bool attempt_hit(Character & attacker, Character & defender);

        // Returns true if Weapon is of WeaponType
        virtual bool is_type(WeaponType type) const;

    protected:
        char *name;
        WeaponType weapon_type;
        AbilityBase ability_base;
        int min_damage;
        int max_damage;
        bool ranged;
        int precision_bonus;
};

// Weapon that deals massive damage, and can knock
// the opponent prone
class Greataxe : public Weapon
{
    public:
        Greataxe();
        Greataxe(const Greataxe & source);

        bool attempt_hit(Character & attacker, Character & defender) override;

    private:

};

// Weapon that attacks twice on attacker's turn
class Dualswords : public Weapon
{
    public:
        Dualswords();
        Dualswords(const Dualswords & source);

        bool attempt_hit(Character & attacker, Character & defender) override;

    private:
        bool second_hit;
};

// Weapon that simply deals damage
class Staff : public Weapon
{
    public:
        Staff();
        Staff(const Staff & source);

        bool attempt_hit(Character & attacker, Character & defender) override;
};

// Weapon that deals damage, ignoring armor
class Dagger : public Weapon
{
    public:
        Dagger();
        Dagger(const Dagger & source);

        bool attempt_hit(Character & attacker, Character & defender) override;
};

// Weapon that deals ranged damage
class Bow : public Weapon
{
    public:
        Bow();
        Bow(const Bow & source);

        bool attempt_hit(Character & attacker, Character & defender) override;
};

// A WeaponList contains all of a Character's Weapons
class WeaponList
{
    public:
        WeaponList();
        WeaponList(const WeaponList & source);
        WeaponList(Weapon** weaps, int ct);
        ~WeaponList();

        // Returns the # of Weapons in the List
        int get_count() const;

        // Inserts a Weapon into the WeaponList
        void insert(const Weapon & weapon);

        // Displays the WeaponList
        void print(bool list) const;

        // Selects Weapon at the specified index
        Weapon *select(int index) const;

    private:
        Weapon **weapons;
        int count;
        int capacity;
};

#endif
