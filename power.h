/*
 * Christian Schmid
 * CS202 - Programming Systems
 * Program 1 - power.h
 * Defines Power class - the collection of special
 * abilities all characters have. Powers are generic, simply
 * taking two Characters and applying some kind of effect to
 * one or both
 */

#ifndef POWER_H_
#define POWER_H_

#include <iostream>
#include <cstring>

#include "character.h"

// Include Character forward declaration due to
// circular header dependencies.
class Character;

class Power
{
    public:
        
        Power(const Power & source);
        Power(const char *nme, int eff_val, 
              const char *flavor_txt, const char *eff_att, 
              const char *eff_succ, const char* eff_fal);

        virtual ~Power();
        virtual Power * clone() const;

        // The key function in class - takes provided data and
        // applies it in some way to Characters (depends on derived class)
        virtual void apply_effect(Character & character, Character & opponent) const = 0;

        // Print the Power's name and flavor-text
        void print_key() const;

    protected:
        char *name;
        int effect_value;
        char *effect_flavortext;
        char *effect_attempt;
        char *effect_success;
        char *effect_fail;

    private:
        // Power cannot be created by the default constructor.
        Power();
};

// Heal restores the caster's health
class Heal : public Power
{
    public:
        Heal(const Heal & source);
        Heal(const char *nme, int eff_val, 
             const char *flavor_txt, const char *eff_att, 
             const char *eff_succ, const char* eff_fal);

        void apply_effect(Character & caster, Character & opponent) const override;        
};

// Fire casts fire from the caster to the opponent,
// damaging armor and health if hit
class Fire : public Power
{
    public:
        Fire(const Fire & source);
        Fire(const char *nme, int eff_val, 
             const char *flavor_txt, const char *eff_att, 
             const char *eff_succ, const char* eff_fal);

        void apply_effect(Character & caster, Character & opponent) const override;
};

// Sharpshoot damages the opponent, and knocks
// them prone
class Sharpshoot : public Power
{
    public:
        Sharpshoot(const Sharpshoot & source);
        Sharpshoot(const char *nme, int eff_val, 
             const char *flavor_txt, const char *eff_att, 
             const char *eff_succ, const char* eff_fal);

        void apply_effect(Character & caster, Character & opponent) const override;
};

// Warcry hits an opponent's Will, decrementing all
// abilities if successful
class Warcry : public Power
{
    public:
        Warcry(const Warcry & source);
        Warcry(const char *nme, int eff_val, 
             const char *flavor_txt, const char *eff_att, 
             const char *eff_succ, const char* eff_fal);

        void apply_effect(Character & caster, Character & opponent) const override;
};

#endif
