/*
 * Christian Schmid
 * CS202 - Programming Systems
 * Program 1 - battle.h
 * Definition of Battle class - runs and
 * stores information on a battle between two
 * Characters. Can trigger Events randomly
 * in the midst of simulations
 */ 

#ifndef BATTLE_H_
#define BATTLE_H_

#include "character.h"
#include "event.h"

class Character;
class Power;

class Battle
{
    public:
        // Constructors
        Battle();
        Battle(const Battle & source);
        Battle(const Character & pl1, const Character & pl2);
        ~Battle();

        // Runs the Battle - prompts Characters to
        // act, until one falls
        int simulate();

    protected:
        // An array of Character *s. Characters are cloned
        // upon Battle construction, to avoid mutating the
        // supplied Character
        Character **characters;

        // The player whose turn it is. Automatically
        // switches from 0 to 1, or vice-versa, in order
        // to select the correct player in characters[]
        int pl_turn;

        // The player whose NOT up this turn. Simple
        // wrapper function to hide logic
        int off_player() const;

        // Checks to see if either (or both) of the
        // players are dead. Returns 0 for pl1, 1 for pl2,
        // 2 for both, or -1 for neither
        int check_for_victor() const;

        // Randomly spawns an Event
        void try_event() const;

        // List of events that can randomly
        // happen during this battle
        Power **events; 
};
#endif