/*
 * Christian Schmid
 * CS202 - Programming Systems
 * Program 1 - weapon.h
 * Implementation of Weapon class, which
 * contains all possible kinds of weapons
 * a Character can use. Also defines WeaponList -
 * a linearly linked list of WeaponTypes. Characters
 * contains WeaponLists as a description of their
 * weapon inventory
 * 
 */

#include "weapon.h"

/* WEAPON MEMBER FUNCTIONS
 * ************************ */
// Default constructor - there shouldn't be an
// instance where this is used, but created for safety
Weapon::Weapon() 
    : name(NULL), weapon_type(NA), ability_base(AbilityBase::STR), 
      min_damage(0), max_damage(0), ranged(false), precision_bonus(0)
{ }

// Weapon Copy Constructor
Weapon::Weapon(const Weapon & from)
    : weapon_type(from.weapon_type), ability_base(from.ability_base), min_damage(from.min_damage), 
      max_damage(from.max_damage), ranged(from.ranged), precision_bonus(from.precision_bonus)
{
    name = new char[strlen(from.name) + 1];
    strcpy(name, from.name);
}

// Weapon Primary Constructor - all derived classes call this
// Constructor
Weapon::Weapon(const char *nme, const WeaponType type, const AbilityBase base, 
               const int min_dmg, const int max_dmg, bool rngd, int prec_bon)
    : weapon_type(type), ability_base(base), min_damage(min_dmg), 
      max_damage(max_dmg), ranged(rngd), precision_bonus(prec_bon)
{
    name = new char[strlen(nme) + 1];
    strcpy(name, nme);
}

// Weapon Destructor
Weapon::~Weapon()
{
    if(name != NULL) delete [] name;
}

// Clones the Weapon, using its WeaponType to 
// dynamic_cast into the correct derivation
Weapon *Weapon::clone() const
{
    switch(weapon_type)
    {
        case GREATAXE: 
            return new Greataxe(*dynamic_cast<const Greataxe*>(this)); 
        case DUALSWORDS:
            return new Dualswords(*dynamic_cast<const Dualswords*>(this));
        case STAFF:
            return new Staff(*dynamic_cast<const Staff*>(this));
        case DAGGER:
            return new Dagger(*dynamic_cast<const Dagger*>(this));
        case BOW:
            return new Bow(*dynamic_cast<const Bow*>(this));
        default:
            return NULL;
    }

}

// Returns true if Weapon is of WeaponType
bool Weapon::is_type(WeaponType type) const
{
    return weapon_type == type;
}

// Attacker attempts to hit the Defender, using Weapon stats
bool Weapon::attempt_hit(Character & attacker, Character & defender)
{
    using namespace std;

    cout << "You attempt to hit "; defender.print(true); 
    cout << " with your " << name << "..." << endl;

    // Roll attacker hit and defender dodge
    // If hit
    if(roll_10() + attacker.get_abl_mod(ability_base) 
     < roll_10() + defender.get_abl_mod(DEX))
    {
        cout << "You hit!" << endl;

        // Deal damage to defender, informing the user of amount
        int damage = rand(min_damage, max_damage + 1) + attacker.get_abl_mod(ability_base);
        cout << "You deal " << defender.take_damage(damage, false) << " damage." << endl;
        return true;
    }
    else
    {
        cout << "You miss!" << endl;
        return false;
    }
    
}

// Displays the Weapon
void Weapon::print() const
{
    std::cout << name;
}

/* ************************** */

// Greataxe Default / Primary Constructor
Greataxe::Greataxe() : Weapon("Greataxe", GREATAXE, STR, 10, 12, false, 0) { }

// Greataxe Copy Constructor
Greataxe::Greataxe(const Greataxe & source) : Weapon(source) { }

// Greataxe attempt_hit has the potential to knock the defender prone
bool Greataxe::attempt_hit(Character & attacker, Character & defender)
{
    if(Weapon::attempt_hit(attacker, defender))
    {
        if(roll_10() + attacker.get_abl_mod(STR) >= 
        roll_10() + defender.get_abl_mod(STR))
        {
            defender.knock_prone();
            defender.print(true); std::cout << " falls flat on their back!" << std::endl;
        }
        return true;
    }
    return false;
}

// Dualswords Default / Primary Constructor
Dualswords::Dualswords() : Weapon("Dualswords", DUALSWORDS, DEX, 6, 8, false, 2), second_hit(false) { }

// Dualswords Copy Constructor
Dualswords::Dualswords(const Dualswords & source) : Weapon(source), second_hit(false) { }

// Dualswords always attack twice on attempt_hit
bool Dualswords::attempt_hit(Character & attacker, Character & defender)
{
    // Hit determines return value - set true
    // if first attack hits
    bool hit = false;
    if(Weapon::attempt_hit(attacker, defender))
    {
        hit = true;
    }

    // Deal another attack if this is 
    // the first strike
    if(!second_hit)
    {
        second_hit = true;
        std::cout << "Dual strike! " << std::endl;
        // Set hit to attempted hit - however, if hit is
        // already true, don't impact value
        hit = attempt_hit(attacker, defender) || hit;
    } 
    else
    {
        second_hit = false;      
    }

    return hit;
}

// Staff Default / Primary Constructor
Staff::Staff() : Weapon("Staff", STAFF, STR, 4, 8, false, 0) { }

// Staff Copy Constructor
Staff::Staff(const Staff & source) : Weapon(source) { }

// Staff deals a simple attack on attempt_hit
bool Staff::attempt_hit(Character & attacker, Character & defender)
{
    return Weapon::attempt_hit(attacker, defender);
}

// Dagger Default / Primary Constructor
Dagger::Dagger() : Weapon("Dagger", DAGGER, DEX, 2, 4, false, 2) { }

// Dagger Copy Constructor
Dagger::Dagger(const Dagger & source) : Weapon(source) { }

// Dagger ignores armor when hit on attempt_hit
bool Dagger::attempt_hit(Character & attacker, Character & defender)
{
    using namespace std;

    cout << "You attempt to hit "; defender.print(true); 
    cout << " with your " << name << "..." << endl;

    // Roll for attack / defense
    // If hit
    if(roll_10() + attacker.get_abl_mod(ability_base) 
     < roll_10() + defender.get_abl_mod(DEX))
    {
        // Inform user of armor ignore
        cout << "You hit! It goes straight through the armor!" << endl;

        // Deal damage and return true
        int damage = rand(min_damage, max_damage + 1) + attacker.get_abl_mod(ability_base);
        cout << "You deal " << defender.take_damage(damage, true) << " damage." << endl;

        return true;
    } 
    else
    {
        cout << "You miss!";
        return false;
    }
    
}

// Bow Default / Primary Constructor
Bow::Bow() : Weapon("Bow", BOW, DEX, 8, 10, true, false) { }

// Bow Copy Constructor
Bow::Bow(const Bow & source) : Weapon(source) { }

// Bow performs simple attack on attempt_hit
bool Bow::attempt_hit(Character & attacker, Character & defender)
{
    return Weapon::attempt_hit(attacker, defender);
}
/* ************************************
 */



/* WEAPON LIST MEMBER FUNCTIONS (in weapon.h)
 * ************************************
 */
// WeaponList Default Constructor
// sets initial capaticty to 4
WeaponList::WeaponList() : count(0), capacity(4) 
{
    weapons = new Weapon*[capacity];
    int i = 0; while(i < capacity) weapons[i] = NULL;
}

// WeaponList Copy Constructor
// Derived classes can create upcasted supplied value
WeaponList::WeaponList(const WeaponList & source) 
    : count(source.count), capacity(source.capacity)
{
    // Copy each Weapon from source into
    // WeaponList
    weapons = new Weapon*[capacity];
    for(int i = 0; i < capacity; ++i)
    {
        if(i < count)
        {
            weapons[i] = source.weapons[i]->clone();
        }
        else
        {
            weapons[i] = NULL;
        }
    }
}

// Array-based Constructor. Takes provided Weapon* array and
// clones all values into WeaponList. Then deletes array
WeaponList::WeaponList(Weapon **weaps, int ct)
    : count(ct), capacity(ct * 2) 
{
    // Allocate memory for Weapon* array
    weapons = new Weapon*[capacity];

    // Clone each Weapon from weaps into WeaponList
    // and set each Weapon* after count to NULL
    for(int i = 0; i < capacity; ++i)
    {
        if(i < count)
        {
            weapons[i] = weaps[i]->clone();
            delete (weaps[i]);
        }
        else
        {
            weapons[i] = NULL;
        }
    }

    // Delete weaps array
    delete [] weaps;
}

// WeaponList Destructor
WeaponList::~WeaponList()
{
    if(weapons != NULL)
    {
        for(int i = 0; i < count; ++i)
        {
            delete (weapons[i]);
        }

        delete [] weapons;
    }
}

// Returns the # of Weapons in the List
int WeaponList::get_count() const { return count; }

// Inserts a Weapon into the WeaponList
void WeaponList::insert(const Weapon & weapon)
{
    // If capacity is reached...
    if(count == capacity)
    {
        // Double capacity
        capacity = capacity * 2;
        // Create new array of Weapon* size of capacity
        Weapon **temp = new Weapon*[capacity]; 

        // Move each Weapon* from array to 
        // new array, and set each after count
        // to NULL
        for(int i = 0; i < capacity; ++i)
        {
            if(i < count)
            {
                temp[i] = weapons[i];
            }
            else
            {
                temp[i] = NULL; 
            }
        }

        // Finally, insert new Weapon
        temp[count++] = weapon.clone();

        // Delete old array, and assign it to new one
        delete [] weapons;
        weapons = temp;
    }
    else
    {
        // If not, simply clone provided Weapon into
        // array
        weapons[count++] = weapon.clone();    
    }
}

// Displays the WeaponList
void WeaponList::print(bool list) const
{
    using namespace std;
    for(int i = 0; i < count; ++i)
    {
        // If list is true, print each Weapon
        // on their own lines, with indices
        if(list)
        {
            cout << i+1 << ") ";
            weapons[i]->print(); 
            cout << endl; 
        }
        // If list is false, print all Weapons
        // on single line, separating by comma
        else      
        {
            weapons[i]->print();
            if(i < count - 1)
            {
                cout << ", ";
            }
            else
            {
                cout << endl;
            }
        }
    }
}

// Selects Weapon at the specified index
Weapon *WeaponList::select(int index) const
{
    // Returns Weapon* at index if index
    // falls within bounds
    if(index >= 0 && index < count)
    {
        return weapons[index];
    }

    // Returns NULL otherwise
    return NULL; 
}
/* ***************************************
 */