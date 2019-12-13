/*
 * Christian Schmid
 * CS202 - Programming Systems
 * Program 1 - characterhandler.h
 * Definition of CharacterHandler, which handles
 * creation, deletion, file reading/writing, and
 * choosing of Characters in the program.
 */

#ifndef CHARACTERHANDLER_H_
#define CHARACTERHANDLER_H_

#include <fstream>

#include "battle.h"
#include "characterlist.h"
#include "magician.h"
#include "ranger.h"
#include "warrior.h"

class CharacterHandler
{
    public:
        CharacterHandler();
        CharacterHandler(const CharacterHandler & source);
        ~CharacterHandler();

        // Primary UI loop that provides the user
        // with a options to add, remove, display, or
        // battle Characters
        void ui_loop();

    private:
        // UI that directs user to create new Character
        void add_char();
        // UI that directs user to remove Character
        void remove_char();
        // UI that directs user to display Character
        void display_char();
        // UI that directs user to remove all Characters
        void remove_all_chars();
        // UI that directs user to battle Characters
        void battle_chars();

        // Loads Characters from file, if it exists
        void load_character(std::ifstream & ifs);

        // Saves Characters to file
        void save_characters();

        CharacterList list;
        Battle battle;

        // Initializes loading procedure
        void load_list();
};

#endif