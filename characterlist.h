/*
 * Christian Schmid
 * CS202 - Programming Systems
 * Program 1 - characterlist.h
 * Definition of a list of CharacterArray,
 * CharacterNode, and CharacterList
 */

#ifndef CHARACTERLIST_H_
#define CHARACTERLIST_H_

#include "character.h"
#include "magician.h"
#include "ranger.h"
#include "warrior.h"

// CharacterArray is a flexible array of Characters
// Follows std::vector capacity style (capacity multiplies
// whenever reached)
class CharacterArray
{
    public:
        CharacterArray();
        CharacterArray(CharacterType type);
        CharacterArray(const CharacterArray & source);
        ~CharacterArray();

        // Tests whether Array contains supplied CharacterType
        bool matches_char_type(const CharacterType type) const;

        // Inserts Character into CharacterArray
        void insert(const Character & character);

        // Removes Character from CharacterArray, by index
        bool remove(const int index);

        // Gets total # of Characters in array
        int get_count() const;

        // Selects Character by index (shallow copy)
        Character *select(int index) const;

        // Display's whole array of Characters
        void print(const bool print_ind, int offset_index) const;

    protected: 
        CharacterType char_type;
        Character **array;
        int count;
        int capacity;
};

// CharacterNode is a CharacterArray, with the addition of
// data members that allow it to be inserted into an LLL
class CharacterNode : public CharacterArray
{
    public:
        CharacterNode();
        CharacterNode(const CharacterType type);
        CharacterNode(const CharacterNode & source);
        ~CharacterNode();

        // Get's the next node in the CLL (by alias)
        CharacterNode *& get_next();
        // Get's the next node, but as readonly - not editable
        const CharacterNode * get_next_readonly() const;

        // Sets the next node in the list (by alias)
        void set_next(CharacterNode *& val);
        
        // Clears the next node in the list
        void clear_next();

    private:
        CharacterNode *next;
};

// CharacterList is a CLL of CharacterNodes which are flexible arrays
// Each node in CharacterList represents a CharacterType, where new
// nodes are added for each new CharacterType added
class CharacterList
{
    public:
        CharacterList();
        CharacterList(const CharacterList & source);
        ~CharacterList();

        // Returns total count of CharacterList
        // including each element in each CharacterArray
        int get_count() const;

        // Displays the whole CharacterList
        void print(const bool print_ind) const;

        // Inserts a new Character into the CharacterList
        void insert(const Character & character);

        // Removes a Character, by index, from the CharacterList
        bool remove(const int index);

        // Selects a Character from the list, by index
        Character *select(const int index);

        // Clears all CharacterNodes from the CLL
        void clear_list();

        // Appends all data to the supplied ofstream
        void save(std::ofstream & ofs);

    private:
        /* PRIVATE RECURSIVE MEMBER FUNCTIONS */
        void copy_from(const CharacterNode *from, CharacterNode *& to, const int ct);
        bool remove_helper(const int offset, CharacterNode *& current);
        void insert_helper(const Character & source, CharacterNode *& current);
        void print_helper(CharacterNode *& current, const bool print_ind, const int offset) const;
        Character *select_helper(CharacterNode *& current, int offset);

        CharacterNode *tail;
        int count;
};

#endif