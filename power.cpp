/*
 * Christian Schmid
 * CS202 - Programming Systems
 * Program 1 - power.h
 * Implementation of Power class - the collection of special
 * abilities all characters have. Powers expend Magic,
 * and either inhibit an opponent (Curse), or help the
 * caster (Boon)
 */

#include "event.h"
#include "power.h"
#include "powerlist.h"

/* POWER MEMBER FUNCTIONS (power.h)
 * ************************
 */
// Power Default Constructor
Power::Power()
    : name(NULL), effect_value(0), 
      effect_flavortext(NULL), effect_attempt(NULL),
      effect_success(NULL), effect_fail(NULL)
{ }

// Power Copy Constructor
Power::Power(const Power & source)
    : effect_value(source.effect_value)
{
    name = new char[strlen(source.name) + 1];
    strcpy(name, source.name);

    effect_flavortext = new char[strlen(source.effect_flavortext) + 1];
    strcpy(effect_flavortext, source.effect_flavortext);

    effect_attempt = new char[strlen(source.effect_attempt) + 1];
    strcpy(effect_attempt, source.effect_attempt);

    effect_success = new char[strlen(source.effect_success) + 1];
    strcpy(effect_success, source.effect_success);

    effect_fail = new char[strlen(source.effect_fail) + 1];
    strcpy(effect_fail, source.effect_fail);
}

// Power Primary Constructor - used in derived classes when
// Constructing shared data
Power::Power(const char *nme, int eff_val, 
    const char *flvr_txt, const char *eff_att, const char *eff_succ, const char *eff_fail)
    : effect_value(eff_val)
{
    name = new char[strlen(nme) + 1];
    strcpy(name, nme);

    effect_flavortext = new char[strlen(flvr_txt) + 1];
    strcpy(effect_flavortext, flvr_txt);

    effect_attempt = new char[strlen(eff_att) + 1];
    strcpy(effect_attempt, eff_att);

    effect_success = new char[strlen(eff_succ) + 1];
    strcpy(effect_success, eff_succ);

    effect_fail = new char[strlen(eff_fail) + 1];
    strcpy(effect_fail, eff_fail);
}

// Power Destructor
Power::~Power()
{
    if(name != NULL) delete [] name;
    if(effect_flavortext != NULL) delete [] effect_flavortext;
    if(effect_attempt != NULL) delete [] effect_attempt;
    if(effect_success != NULL) delete [] effect_success;
    if(effect_fail != NULL) delete [] effect_fail;
}

// Clones a Power - uses upcasting by cloning
// derived class. Uses dynamic_cast to determine
// what derived class the Power is
Power *Power::clone() const
{
    // Heal check
    using namespace std;
    const Heal *heal = dynamic_cast<const Heal*>(this);
    if(heal != NULL) return new Heal(*heal);

    // Fire check
    const Fire *fire = dynamic_cast<const Fire*>(this);
    if(fire != NULL) return new Fire(*fire);

    // Sharpshoot check
    const Sharpshoot *sharpshoot = dynamic_cast<const Sharpshoot*>(this);
    if(sharpshoot != NULL) return new Sharpshoot(*sharpshoot);

    // Warcry check
    const Warcry *warcry = dynamic_cast<const Warcry*>(this);
    if(warcry != NULL) return new Warcry(*warcry);

    // If none are a match, return NULL
    return NULL;
}

// Print the Power's name and flavor-text
void Power::print_key() const
{
    std::cout << name << std::endl;
    std::cout << "  >> " << effect_flavortext << std::endl;
}

// Heal Copy Constructor
Heal::Heal(const Heal & source) : Power(source) { }

// Heal Primary Constructor
Heal::Heal(const char *nme, int eff_val, 
             const char *flavor_txt, const char *eff_att, 
             const char *eff_succ, const char* eff_fal)
             : Power(nme, eff_val, flavor_txt, eff_att,
                     eff_succ, eff_fal) { }

// Heal's apply_effect() heals the caster by the effect_value's amount
void Heal::apply_effect(Character & caster, Character & opponent) const
{
    using namespace std;
    cout << effect_attempt << endl;
    cout << effect_success << endl;
    cout << "You are restored " << effect_value << " HP." << endl;

    caster.restore_health(effect_value);
}

// Fire Copy Constructor
Fire::Fire(const Fire & source) : Power(source) { }

// Fire's Primary Constructor
Fire::Fire(const char *nme, int eff_val, 
             const char *flavor_txt, const char *eff_att, 
             const char *eff_succ, const char* eff_fal)
             : Power(nme, eff_val, flavor_txt, eff_att,
                     eff_succ, eff_fal) { }

// With Fire's apply_effect, the caster attempts to throw fire
// at the opponent. Both the caster and opponent roll for attack/dodge
void Fire::apply_effect(Character & caster, Character & opponent) const
{
    using namespace std;
    cout << effect_attempt << endl;

    // Roll for attack/defense
    // If hit
    if(roll_10() + caster.get_abl_mod(AbilityBase::WILL) > 
       roll_10() + opponent.get_abl_mod(AbilityBase::DEX))
    {
        // Opponent takes effect_value armor damage
        opponent.take_armor_damage(effect_value);
        // Opponent takes 1/2 effect_value damage (ignores armor)
        opponent.take_damage(effect_value / 2, true);
        cout << effect_success << endl;
        cout << "Your opponent take " << effect_value / 2;
        cout << " damage and " << effect_value << " armor damage." << endl;
    }
    else
    {
        cout << effect_fail << endl;
    }
}

// Sharpshoot Copy Constructor
Sharpshoot::Sharpshoot(const Sharpshoot & source) : Power(source) { }

// Sharpshoot Primary Constructor
Sharpshoot::Sharpshoot(const char *nme, int eff_val, 
             const char *flavor_txt, const char *eff_att, 
             const char *eff_succ, const char* eff_fal)
             : Power(nme, eff_val, flavor_txt, eff_att,
                     eff_succ, eff_fal) { }

// With Sharpshoot's apply_effect, caster attempts to hit opponent
// while opponent attempts to dodge
void Sharpshoot::apply_effect(Character & caster, Character & opponent) const
{
    using namespace std;

    cout << effect_attempt << endl;
    // Roll for attack/defense
    // If hit
    if(roll_10() + caster.get_abl_mod(AbilityBase::DEX) >
       roll_10() + opponent.get_abl_mod(AbilityBase::DEX))
    {
        // Opponent takes damage, and get's knocked prone
        opponent.knock_prone();
        
        cout << effect_success << endl;
        cout << "You deal " << opponent.take_damage(effect_value, false) << " damage." << endl;
    }
    else
    {
        cout << effect_fail << endl;
    }
}

// Warcry Copy Constructor
Warcry::Warcry(const Warcry & source) : Power(source) { }

// Warcry Primary Constructor
Warcry::Warcry(const char *nme, int eff_val, 
             const char *flavor_txt, const char *eff_att, 
             const char *eff_succ, const char* eff_fal)
             : Power(nme, eff_val, flavor_txt, eff_att,
                     eff_succ, eff_fal) { }

// On Warcry's apply_effect, caster attempts to damage opponents Ability scores
void Warcry::apply_effect(Character & caster, Character & opponent) const
{
    using namespace std;

    cout << effect_attempt << endl;

    // Roll to hit/defend
    // If hit
    if(roll_10() + caster.get_abl_mod(AbilityBase::STR) >
       roll_10() + opponent.get_abl_mod(AbilityBase::WILL))
    {
        // Decrement all opponent's ability by effect_value
        cout << effect_success << endl;
        opponent.decrement_ability(AbilityBase::WILL, effect_value);
        opponent.decrement_ability(AbilityBase::STR, effect_value);
        opponent.decrement_ability(AbilityBase::DEX, effect_value);
    }
    else
    {
        cout << effect_fail << endl;
    }
}
/* **************************
 */



/* EVENT MEMBER FUNCTIONS (events.h)
 * ***************************
 */
// EventElephantStampede Default / Primary Constructor
EventElephantStampede::EventElephantStampede()
    : Power("Elephant Stampede", 10, "",
      "The king grows weary! He releases the stampeding elephants...",
      "", "")
{ }

// EventElephantStampede Copy Constructor
EventElephantStampede::EventElephantStampede(const EventElephantStampede & source)
    : Power(source)
{ }

// EventElephantStampede attempts to damage the players with a
// group of stampeding elephants (needless to say)
void EventElephantStampede::apply_effect(Character & char1, Character & char2) const
{
    using namespace std;
    cout << effect_attempt << endl;
    wait_for_enter();

    // Roll attempt
    int roll = roll_10();

    // Player 1 attempts dodge
    char1.print(true);
    if(char1.attempt_dodge(DEX, roll + 5))
    {
        // Inform user if dodged
        cout << " weaved through the elephants unscathed!" << endl;
    }
    else
    {
        // Inform user if hit, and take effect_value damage
        cout << " is trampled! " << effect_value << " damage!" << endl;     
        char1.take_damage(effect_value, true);
    }
    wait_for_enter();

    // Player 2 attempts dodge... 
    char2.print(true);
    if(char2.attempt_dodge(DEX, roll + 5))
    {
        cout << " weaved through the elephants unscathed!" << endl;
    }
    else
    {
        cout << " is trampled! " << effect_value << " damage!" << endl;     
        char2.take_damage(effect_value, true);
    }
    wait_for_enter();   
}

// EventEarthquake Default / Primary Constructor
EventEarthquake::EventEarthquake()
    : Power("Earthquake", 0, "", 
      "The ground suddenly begins to rumble...",
      "", "") 
{ }

// EventEarthquake Copy Constructor
EventEarthquake::EventEarthquake(const EventEarthquake & source)
    : Power(source) { }

// EventEarthquake's apply_effect attempts to knock the Characters prone
void EventEarthquake::apply_effect(Character & char1, Character & char2) const
{
    using namespace std;
    cout << effect_attempt << endl;
    wait_for_enter();

    // Attack roll
    int roll = roll_10();

    // Player 1 attempts dodge
    char1.print(true);
    if(char1.attempt_dodge(DEX, roll + 5))
    {
        // If successful Player1 doesn't get knocked prone
        cout << " remained standing." << endl;
    }
    else
    {
        // If not successful Player 1 is knocked prone
        cout << " is knocked prone!" << endl;
        char1.knock_prone();
    }
    wait_for_enter();

    // Player 2 attempts dodge 
    char2.print(true);
    if(char2.attempt_dodge(DEX, roll + 5))
    {
        cout << " remained standing" << endl;
    }
    else
    {
        cout << " is knocked prone!" << endl;
        char2.knock_prone();
    }
    wait_for_enter();
}
/* ************************************
 */




/* POWERLIST MEMBER FUNCTIONS (powerlist.h)
 * ****************************
 */
// PowerList Default Constructor
PowerList::PowerList()
    : data(NULL), count(0) { }

// PowerList Power Array Constructor
// Takes a list of Power* and clones them
// into the PowerList. Then deletes the Power* array
// to prevent memory leaking
PowerList::PowerList(Power **pwrs, const int ct)
    : count(ct)
{
    data = new Power*[count];
    for(int i = 0; i < count; ++i)
    {
        data[i] = pwrs[i]->clone();
        delete (pwrs[i]);
    } 
    delete [] pwrs;
}

// PowerList Copy Constructor
// clones all Powers into list
PowerList::PowerList(const PowerList & source)
    : count(source.count)
{
    data = new Power*[count];
    for(int i = 0; i < count; ++i)
    {
        data[i] = source.data[i]->clone();
    }
}

// PowerList Destructor
PowerList::~PowerList()
{
    if(data != NULL)
    {
        for(int i = 0; i < count; ++i)
        {
            delete (data[i]);
        }
        delete [] data;
    }
}

// Retrieves Power at specified index, if index
// falls within bounds. Returns NULL if it doesn't
const Power* PowerList::get(int index) const
{
    if(index < 0 || index >= count) return NULL;
    return data[index];
}

// Returns # of elements in the PowerList
int PowerList::get_count() const { return count; }

