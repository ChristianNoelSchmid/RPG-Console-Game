/*
 * Christian Schmid
 * CS202 - Programming Systems
 * Program 1 - character.h
 * 
 * Implementation of classes Stats, Character, Magician,
 * Ranger, and Warrior
 */

#include "stats.h"
#include "character.h"
#include "magician.h"
#include "ranger.h"
#include "warrior.h"

/* STATS MEMBER FUNCTIONS (stats.h)
 * ********************************
 */
// Stats Default Constructor
Stats::Stats() 
    : exp(0), health(0), stamina(0), magic(0), armor(0),
      base_str(0), base_dex(0), base_will(0)

    { }

// Stats Primary Constructor - used to template each
// different class' Stats (see magician.h, ranger.h, warrior.h)
Stats::Stats(const int strength, const int dexterity, const int will, const int armr)
    : exp(0), armor(armr), base_str(strength), base_dex(dexterity), base_will(will)
{
    // Set battle stats to max
    health = get_max_health();
    stamina = get_max_stam();
    magic = get_max_mag();
}

// Displays all Stats
void Stats::print() const
{
    using namespace std;
    cout << "Stats:" << endl << endl;
    cout << "\tSTR: " << get_str() << ", DEX: " << get_dex() << ", WILL: " << get_will() << endl;
    cout << endl;
    cout << "\tHealth: " << health << endl;
    cout << "\tStamina: " << stamina << endl;
    cout << "\tMagic: " << magic << endl << endl;
    cout << "\tArmor: " << armor << endl << endl;
    cout << "Experience: " << exp << endl << endl;
}

// Adds experience to Stats. Affects all
// ability getters, and resets health, stamina, and magic
void Stats::add_exp(int to_add)
{
    exp += to_add;
    health = get_max_health();
    stamina = get_max_stam();
    magic = get_max_mag();
}

// Ability getters take their bases, and multiply them by 1.(exp * 0.01)
// 1 is assumed, so that the minimum will always be the base value (even
// if exp is 0)
int Stats::get_str() const { return base_str * ((exp * 0.01) + 1); }
int Stats::get_dex() const { return base_dex * ((exp * 0.01) + 1); }
int Stats::get_will() const { return base_will * ((exp * 0.01) + 1); }

// Max stat getters take their main ability value, multiplied by 1.5
// and add a quarter of the secondary ability value:
int Stats::get_max_health() const { return 15 + (get_str() * 1.5) + (get_will() / 4); }
int Stats::get_max_stam() const { return (get_dex() * 1.5) + (get_str() / 4); }
int Stats::get_max_mag() const { return (get_will() * 1.5) + (get_dex() / 4); }

// Return ability modifier, impacted by Stats stamina
// Used in combat - attacking and dodging
// Abilities are quarters, then fractioned by stamina
// loss
int Stats::get_abl_mod(const AbilityBase base) const
{
    if(base == AbilityBase::STR)
    {
        return (get_str() / 4) * ((stamina / get_max_stam()) + 0.5);
    }
    else if(base == AbilityBase::DEX)
    {
        return (get_dex() / 4) * ((stamina / get_max_stam()) + 0.5);
    }
    else
    {
        return (get_will() / 4) * ((stamina / get_max_stam()) + 0.5);
    }
}

// Restores health by specified amount
void Stats::restore_health(int value)
{
    // Health cannot go above 
    // max_health value
    health += value;
    if(health > get_max_health())
    {
        health = get_max_health();
    }
}

// Restores stamina back to max amount
void Stats::restore_stamina()
{
    stamina = get_max_stam();
}

// Subtracts specified amount from ability
void Stats::decrement_ability(const AbilityBase base, const int value)
{
    if(base == AbilityBase::STR)
    {
        base_str -= value;
    }
    else if(base == AbilityBase::DEX)
    {
        base_dex -= value;
    }
    else
    {
        base_will -= value; 
    }    
}


// Returns Stats experience value
// impacted by Stats current experience
int Stats::get_exp_value() const
{
    return (14 + exp);
}

// Subtract health, blocked by armor, unless ign_armor is true
int Stats::take_damage(const int value, const bool ign_armor)
{
    int total;
    if(ign_armor)
    {
        total = value;
    }
    else
    {
        // Never allow total to be negative
        // (that would add health!)
        total = at_least_0(value - armor);
    }
    health -= total;
    return total; 
}

// Subtract armor by value, or to 0
void Stats::take_armor_damage(const int value)
{
    armor = at_least_0(armor - value);
}
/* ***************************
 */



/* CHARACTER MEMBER FUNCTIONS (in character.h)
 * ***************************
 */
// Character Default Constructor
Character::Character()
    : char_type(WARRIOR), name(NULL), is_prone(false), weapon_list(new WeaponList()), 
      power_list(new PowerList()), win_list(NULL), loss_list(NULL) { }

// Character Copy Constructor
Character::Character(const Character & copy_from) 
    : Stats(copy_from), char_type(copy_from.char_type), is_prone(copy_from.is_prone), 
      weapon_list(new WeaponList(*(copy_from.weapon_list))), 
      power_list(new PowerList(*(copy_from.power_list))), 
      win_list(new BattleHistoryList(*(copy_from.win_list))), 
      loss_list(new BattleHistoryList(*(copy_from.loss_list)))
{
    name = new char[strlen(copy_from.name) + 1];
    strcpy(name, copy_from.name);
}

// Character Primary Constructor - Derived classes call this Constructor to
// set base data
Character::Character(const CharacterType type, const char *nme, const Stats stats, 
                     const WeaponList & wpn_lst, const PowerList & power_ls) 
    : Stats(stats), char_type(type), is_prone(false), weapon_list(new WeaponList(wpn_lst)), 
      win_list(new BattleHistoryList()), loss_list(new BattleHistoryList())
{
    name = new char[strlen(nme) + 1];
    strcpy(name, nme);

    power_list = new PowerList(power_ls);
}

// Character Destructor
Character::~Character()
{
    delete [] name;
    delete power_list;
    delete weapon_list;
    delete win_list;
    delete loss_list;
}

// Determines if Character is equal to supplied Character
// comparing names
bool Character::equals(const Character & other) const
{
    return other.char_type == char_type && strcmp(other.name, name) == 0;
}

// Displays Character - either just the name (name_only)
// or base class Stats as well
void Character::print(const bool name_only) const
{
    using namespace std;
    cout << name;

    if(!name_only)
    {
        cout << endl;
        Stats::print();
        cout << "Weapons: ";
        weapon_list->print(false);
        cout << endl;

        print_history();

        cout << endl;
    }
}

// Provides the CharacterType - used in ADT CharacterList implementations
CharacterType Character::character_type() const { return char_type; }

// Prompt user input for Character action against
// opponent Character
void Character::prompt_action(Character & character)
{
    using namespace std;

    // Increment magic
    magic += get_abl_mod(WILL);

    // Print Character Stats
    clear_screen();
    print(true); cout << ". Your Stats: ";
    cout << "(HP: " << health << ", STM: " << stamina << ", MAG: " << magic << ")" << endl;
    cout << "Armor: " << armor << endl;

    // If Character is prone, simply stand up and 
    // end turn
    if(is_prone)
    {
        print(true); cout << " stands up..." << endl;
        is_prone = false;
        wait_for_enter();
        return;
    }

    // Print list of options
    cout << "Choose an action (enter #1-5): " << endl;
    cout << "  1) Attack with Weapon (spends Stamina)" << endl;
    cout << "  2) Use Power (spends Magic)" << endl;
    cout << "  3) Wait (Restores Stamina)" << endl;
    cout << "  4) View Fighter Stats..." << endl;
    cout << "--" << endl;

    int input = get_int_input(5);

    // If Attack with Weapon
    if(input == 1)
    {
        // Show WeaponList
        clear_screen();
        cout << "Choose your weapon: " << endl << "--" << endl;
        weapon_list->print(true);

        // Attack with selected Weapon
        input = get_int_input(weapon_list->get_count());
        weapon_list->select(input - 1)->attempt_hit(*this, character);

        // Subtract Character Stamina
        stamina = at_least_0(stamina - 10);
        wait_for_enter();
    }
    // If Use Power
    else if(input == 2)
    {
        // Return if magic is not high enough (requires 10)
        if(magic < 10)
        {
            cout << "You do not have enough magic to cast a power!" << endl;
            wait_for_enter();

            prompt_action(character);
        }
        else
        {
            // Show PowerList, and retrieve selection
            clear_screen();
            cout << "Choose a power:" << endl << "--" << endl;
            for(int i = 0; i < power_list->get_count(); ++i)
            {
                cout << i + 1 << ") ";
                power_list->get(i)->print_key();
            }
            cout << "--" << endl;
            int input = get_int_input(power_list->get_count());

            // Attempt to cast power, and subtract magic
            cast_power(input - 1, character);
            magic -= 10;
            wait_for_enter();
        }
    }
    // If Wait
    else if(input == 3)
    {
        // Display note that stamina is restored, and end turn
        cout << "You wait, restoring your stamina..." << endl;
        restore_stamina();
        wait_for_enter();
    }
    // If View Fighter Stats...
    else if(input == 4)
    {
        // Print both Characters, and return
        clear_screen();

        cout << "Fighter 1: ";
        print(false);
        cout << "--" << endl;

        cout << "Fighter 2: ";
        character.print(false);
        cout << "--" << endl;

        wait_for_enter();
        prompt_action(character);
    }
}

// Attempts to dodge attack 
bool Character::attempt_dodge(const AbilityBase base, int roll_value) const
{
    // Return proper ability value for dodge attempt,
    // vs. roll value
    if(base == AbilityBase::STR)
    {
        return get_str() > roll_value;
    }
    else if(base == AbilityBase::DEX)
    {
        return get_dex() > roll_value;
    }
    else
    {
        return get_will() > roll_value;
    }
}

// Knock the Character prone
void Character::knock_prone() { is_prone = true; }

// Determines if Character's hp is below 0
bool Character::is_dead() const { return health <= 0; }

// Casts a power the Character has, against supplied Character opponent
bool Character::cast_power(int index, Character & opponent)
{
    if(index < 0 || index >= power_list->get_count())
        return false;

    power_list->get(index)->apply_effect(*this, opponent);
    return true;
}

// Prints the Character's list of BattleHistories
void Character::print_history() const
{
    // Print wins if any
    std::cout << "WINS:\n--\n";
    if(win_list->get_count() > 0)
    {
        win_list->print();
    }
    else
    {
        std::cout << "NO WINS\n";
    }

    // Then print losses if any 
    std::cout << "LOSSES:\n--\n";
    if(loss_list->get_count() > 0)
    {
        loss_list->print();
    }
    else
    {
        std::cout << "NO LOSSES\n";
    }
}

// Inserts a won Battle into the Character's BattleHistory
void Character::insert_win(const Character & opponent, int exp)
{
    win_list->insert(BattleHistory(true, opponent.name, exp));
}


// Inserts a lost Battle into the Character's BattleHistory
void Character::insert_loss(const Character & opponent, int exp)
{
    loss_list->insert(BattleHistory(false, opponent.name, exp));
}

// Takes an ifstream and copies data into BattleHistory lists,
// seperating wins and losses
void Character::load_battle_history(std::ifstream & ifs)
{
    // Create temp char*'s for win, exp and name
    char temp[READ_WIDTH];
    char name[READ_WIDTH];
    int exp;

    // While end-of-line is not reached
    while(ifs && ifs.peek() != '\n')
    {
        // Get win value
        ifs.get(temp, READ_WIDTH, '|');
        ifs.ignore(READ_WIDTH, '|'); 

        bool win = atoi(temp);

        // Get name value
        ifs.get(name, READ_WIDTH, '|');
        ifs.ignore(READ_WIDTH, '|');

        // Get experience value
        ifs.get(temp, READ_WIDTH, '|');
        ifs.ignore(READ_WIDTH, '|');
        exp = atoi(temp);

        // Insert data into proper BattleHistoryList, based on if win
        if(win)
        {
            win_list->insert(BattleHistory(true, name, exp));
        }
        else
        {
            loss_list->insert(BattleHistory(false, name, exp));
        }
    } 
}

// Appends Character data to the provided ofstream
void Character::save(std::ofstream & ofs)
{
    // Append CharacterType (casted to int),
    // name, and experience
    int char_tpe = static_cast<int>(char_type);
    ofs << char_tpe << "|";
    ofs << name << "|";
    ofs << exp << "|";

    // Append win BattleHistoryList
    if(win_list->get_count() > 0)
    {
        win_list->save(ofs);
    }

    // Append loss BattleHistoryList
    if(loss_list->get_count() > 0)
    {
        loss_list->save(ofs);
    }
}
/* ***********************
 */



/* MAGICIAN MEMBER FUNCTIONS (in magician.h)
 * ******************************
 */
// Default and Primary Constructor - creates a default Magician
Magician::Magician() :
    Character(MAGICIAN, "", MAG_STATS, MAG_WEAPS, MAG_POWERS)
    { }

// Copy Constructor
Magician::Magician(const Magician & source)
    : Character(source) { }

// Constructor when name's provided
Magician::Magician(const char *nme)
    : Character(MAGICIAN, nme, MAG_STATS, MAG_WEAPS, MAG_POWERS)
    { }

// Constructor called when loading data
Magician::Magician(const char *nme, int exp)
    : Character(MAGICIAN, nme, MAG_STATS, MAG_WEAPS, MAG_POWERS)
{
    add_exp(exp); 
}

// Clones Magician, upcasting to Character*
Character *Magician::clone() const { return new Magician(*this); }

// Display's Magician
void Magician::print(const bool name_only) const
{
    std::cout << "Magician ";
    Character::print(name_only);
}
/* **************************
 */



/* RANGER MEMBER FUNCTIONS (in ranger.h)
 * ***************************
 */
// Default and Primary Constructor - creates a default Ranger
Ranger::Ranger() :
    Character(RANGER, "", RAN_STATS, RAN_WEAPS, RAN_POWERS)
    { }

// Copy Constructor
Ranger::Ranger(const Ranger & source)
    : Character(source) { }

// Constructor when name's provided
Ranger::Ranger(const char *nme)
    : Character(RANGER, nme, RAN_STATS, RAN_WEAPS, RAN_POWERS)
{ }

// Constructor called when loading data
Ranger::Ranger(const char *nme, int exp)
    : Character(RANGER, nme, RAN_STATS, RAN_WEAPS, RAN_POWERS)
{
    add_exp(exp);
}

// Clones Ranger, upcasting to Character*
Character *Ranger::clone() const { return new Ranger(*this); }

// Display's Ranger
void Ranger::print(const bool name_only) const
{
    std::cout << "Ranger ";
    Character::print(name_only);
}
/* ******************************
 */



/* WARRIOR MEMBER FUNCTIONS (in warrior.h)
 * *************************
 */
// Default and Primary Constructor - creates a default Warrior
Warrior::Warrior() 
    : Character(WARRIOR, "", WARRIOR_STATS, WARRIOR_WEAPS, WARRIOR_POWERS)
    { }

// Copy Constructor
Warrior::Warrior(const Warrior & source)
    : Character(source) { }

// Constructor when name's provided
Warrior::Warrior(const char *nme)
    : Character(WARRIOR, nme, WARRIOR_STATS, WARRIOR_WEAPS, WARRIOR_POWERS) 
{ }

// Constructor called when loading data
Warrior::Warrior(const char *nme, int exp)
    : Character(WARRIOR, nme, WARRIOR_STATS, WARRIOR_WEAPS, WARRIOR_POWERS)
{
    add_exp(exp);
}

// Clones Warrior, upcasting to Character*
Character *Warrior::clone() const { return new Warrior(*this); }

// Display's Warrior
void Warrior::print(bool name_only) const
{
    std::cout << "Warrior ";
    Character::print(name_only);
}
/* ******************************
 */