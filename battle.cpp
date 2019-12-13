/*
 * Christian Schmid
 * CS202 - Programming Systems
 * Program 1 - battle.h
 * Implementation of Battle class - runs and
 * stores information on a battle between two
 * Characters. Can trigger Events randomly
 * in the midst of simulations
 */

#include "battle.h"
#include "battlehistory.h"

/* BATTLE MEMBER FUNCTIONS (in battle.h) *
 *************************************
 */
// Default Constructor
Battle::Battle() : characters(NULL), pl_turn(0) 
{ 
    // Assign Events to the events variable
    events = new Power*[2]
    {
        new EventElephantStampede(),
        new EventEarthquake()
    };
}

// Copy Constructor
Battle::Battle(const Battle & source) : pl_turn(source.pl_turn)
{
    // Clone characters
    characters = new Character*[2];
    characters[0] = source.characters[0]->clone();
    characters[1] = source.characters[1]->clone();

    // Assign Events to the events variables
    events = new Power*[2]
    {
        new EventElephantStampede(),
        new EventEarthquake()
    };
}

// Primary Constructor - requires 2 Characters that will
// be battled together
Battle::Battle(const Character & pl1, const Character & pl2)
{
    // Clone over Characters into characters[]
    // this ensures that original Characters are not
    // affected by Battle
    characters = new Character*[2];
    characters[0] = pl1.clone();
    characters[1] = pl2.clone();

    // Determine who goes first based on Dexterity value
    pl_turn = characters[0]->get_abl_mod(DEX) >= characters[1]->get_abl_mod(DEX)
        ? 0 : 1;

    // Assign Events to the events variables
    events = new Power*[2]
    {
        new EventElephantStampede(),
        new EventEarthquake()
    };
}

// Battle Destructor
Battle::~Battle()
{
    // Delete both Characters, and the array
    if(characters != NULL)
    {
        for(int i = 0; i < 2; ++i)
        {
            delete (characters[i]);
        }
        delete [] characters;
    }

    // Delete all Events
    for(int i = 0; i < 2; ++i)
    {
        delete (events[i]);
    }
    delete [] events;
}

// Runs the Battle - prompts Characters to
// act, until one fallsd
int Battle::simulate()
{
    using namespace std;

    // victor is used to determine when there is a winner
    int victor = -1;
    while(victor == -1)
    {
        // Each Character takes a turn, with victor being checked
        // each action
        for(int i = 0; i < 2 && victor == -1; ++i)
        {
            pl_turn = i;

            characters[pl_turn]->prompt_action(*characters[off_player()]);

            victor = check_for_victor();
        }

        // If there is still no victor at end of round,
        // attempt an Event (random chance)
        if(victor == -1)
        {
            try_event();
            victor = check_for_victor();
        }
    }

    return victor;
}

// Determines which player is NOT up. Returns the opposite
// of whatever pl_turn is (can be 0 or 1)
int Battle::off_player() const { return pl_turn == 0 ? 1 : 0; }

// Checks to see if either (or both) of the
// players are dead. Returns 0 for pl1, 1 for pl2,
// 2 for both, or -1 for neither
int Battle::check_for_victor() const
{
    using namespace std;
    // First, check if both have died
    if(characters[0]->is_dead() && characters[1]->is_dead())
    {
        cout << "Both fighters have fallen! It's a draw." << endl;
        return 2;
    }
    // If not, determine if one of them has died
    else
    {
        for(int i = 0; i < 2; ++i)
        {
            // If so, print text and return appropriate
            // value
            if(characters[i]->is_dead())
            {
                int other = (i == 0 ? 1 : 0);

                characters[i]->print(true);
                cout << " has fallen! Congratulations ";
                characters[other]->print(true); cout << "!" << endl;
                wait_for_enter();

                return other;
            }
        }
    }
    return -1;
}

// Randomly spawns an Event
void Battle::try_event() const
{
    // First, randomize whether an Event actually happens
    if(rand(0, 100) > 65)
    {
        // If so, spawn one of the Events (randomly chosen)
        int choice = rand(0, 100) % 2;
        Power *chosen_event = events[choice];
        chosen_event->apply_effect(*characters[0], *characters[1]);
    }
}
/* ****************************
 */



/* BATTLEHISTORY MEMBER/FUNCTIONS (in battlehistory.h)
 * ******************************
 */
// BattleHistory Default Constructor
BattleHistory::BattleHistory() 
    : win(false), opponent_name(NULL), exp_gained(0) { }

// BattleHistory Copy Constructor
BattleHistory::BattleHistory(const BattleHistory & source)
    : win(source.win), exp_gained(source.exp_gained)
{
    opponent_name = new char[strlen(source.opponent_name) + 1];
    strcpy(opponent_name, source.opponent_name);
}

// BattleHistory Primary Constructor - requires win value, opponent name, and experience gained
BattleHistory::BattleHistory(const bool did_win, const char * op_name, const int exp)
    : win(did_win), exp_gained(exp)
{
    opponent_name = new char[strlen(op_name) + 1];
    strcpy(opponent_name, op_name);
}

// BattleHistory Destructor
BattleHistory::~BattleHistory()
{
    // Delete opponent name
    if(opponent_name != NULL)
    {
        delete [] opponent_name;
    }
}

// Prints the BattleHistory instance
void BattleHistory::print(const int offset) const
{
    using namespace std;
    cout << offset << ") " << "Battle with " << opponent_name << ": " << endl;
    cout << "  Battle " << (win ? "Won" : "Lost") << endl;
    cout << "  Experience Gained: " << exp_gained << endl;
}

// Copies contents of provided BattleHistory
void BattleHistory::copy_from(const BattleHistory & source)
{
    // First delete opponent_name, if it's been allocated
    if(opponent_name != NULL)
    {
        delete [] opponent_name;
    } 

    // Copy over all data
    opponent_name = new char[strlen(source.opponent_name) + 1];
    strcpy(opponent_name, source.opponent_name);

    win = source.is_win();
    exp_gained = source.exp_gained;
}

// Returns whether this BattleHistory is a win
// used in BattleHistoryList ADTs, to seperate wins
// and losses
bool BattleHistory::is_win() const { return win; }

// Appends BattleHistory information to the
// supplied ofstream
void BattleHistory::save(std::ofstream & ofs)
{
    ofs << win << "|";
    ofs << opponent_name << "|";
    ofs << exp_gained << "|";
}



// BattleHistoryNode Default Constructor
BattleHistoryNode::BattleHistoryNode()
    : next(NULL) { }

// BattleHistoryNode Copy Contructor
BattleHistoryNode::BattleHistoryNode(const BattleHistoryNode & source) 
    : BattleHistory(source), next(NULL) { }

// BattleHistoryNode Primary Constructor - copies a BattleHistory
// instance
BattleHistoryNode::BattleHistoryNode(const BattleHistory & source)
    : BattleHistory(source), next(NULL) { }

// BattleHistoryNode Destructor
// Recursive deletion
BattleHistoryNode::~BattleHistoryNode() { delete next; }

// Constructor that calls BattleHistory Primary Constructor, providing
// values
BattleHistoryNode::BattleHistoryNode(const bool did_win, const char *op_name, const int exp)
    : BattleHistory(did_win, op_name, exp), next(NULL) { }

// Retrieves the next BattleHistoryNode in the LLL
BattleHistoryNode *& BattleHistoryNode::get_next() { return next; }

// Retrieves the next node, but as a const, so it
// cannot be edited
const BattleHistoryNode *BattleHistoryNode::get_next_readonly() const { return next; }

// Sets the next BattleHistoryNode in the LLL
void BattleHistoryNode::set_next(BattleHistoryNode *& source)
{
    next = source;
}
//Clears the next element
void BattleHistoryNode::clear_next()
{
    next = NULL;
}



// BattleHistoryList Default Constructor
BattleHistoryList::BattleHistoryList()
    : head(NULL), count(0) { }

// BattleHistoryList Copy Constructor
BattleHistoryList::BattleHistoryList(const BattleHistoryList & source)
    : head(NULL), count(0)
{
    copy_list(source.head, head);
}

// BattleHistoryList Destructor
BattleHistoryList::~BattleHistoryList()
{
    delete head;
}

// Returns the # of elements in the LLL
int BattleHistoryList::get_count() const { return count; }

// Inserts a new BattleHistory into the LLL
void BattleHistoryList::insert(const BattleHistory & history)
{
    insert_helper(history, head);
}

// Removes a BattleHistory from the LLL (by index)
bool BattleHistoryList::remove(const int index)
{
    return remove_helper(index, head);
}

// Select a BattleHistory from the list, by index
bool BattleHistoryList::select(const int index, BattleHistory & write_to)
{
    return select_helper(index, write_to, head);
}

// Clears the entire BattleHistoryList of elements
void BattleHistoryList::clear()
{
    delete head;
    head = NULL;
    count = 0;
}

// Displays the BattleHistoryList
// calls recursive helper function print_helper()
void BattleHistoryList::print() const
{
    print_helper(head, 1);
}

// Appends LLL to supplied ofstream
// calls recursive helper function save_helper()
void BattleHistoryList::save(std::ofstream & ofs)
{
    save_helper(ofs, head);    
}

/* PRIVATE RECURSIVE MEMBER FUNCTIONS 
 */
// Copies node for node an entire LLL to this one 
// Used in Copy Constructor
void BattleHistoryList::copy_list(const BattleHistoryNode *from, BattleHistoryNode *& to)
{
    if(from != NULL)
    {
        ++count;
        to = new BattleHistoryNode(*from);
        copy_list(from->get_next_readonly(), to->get_next());
    }
}

// Traverses the LLL until it meets the end
// then copies history over to NULL node
void BattleHistoryList::insert_helper(const BattleHistory & history, BattleHistoryNode *& current)
{
    if(current == NULL)
    {
        current = new BattleHistoryNode(history);
        ++count;
    }
    else
    {
        insert_helper(history, current->get_next());
    }
    
}

// Traverses the LLL until index is 0, then removes the index
// Returns false if end of list is reached before index reaches 0
bool BattleHistoryList::remove_helper(const int index, BattleHistoryNode *& current)
{
    // If current is NULL, index was too great value - return false
    if(current == NULL)
    {
        return false;
    }
    // If current is not NULL and index is 0, value found - remove
    else if(index == 0)
    {
        BattleHistoryNode *temp = current;
        current = current->get_next();

        temp->clear_next();
        delete temp;

        return true;
    }
    // Otherwise, continue traversal, decrementing index
    else
    {
        return remove_helper(index - 1, current->get_next());
    }
}

// Traverses the LLL until index is 0, then writes element to write_to
// Returns false if end of list is reached before index reaches 0
bool BattleHistoryList::select_helper(const int index, BattleHistory & write_to, BattleHistoryNode *current) const
{
    // If current is NULL, index was too high value - return false
    if(current == NULL)
    {
        return false;
    }
    // If current is not NULL and index is 0, value found - write to
    else if(index == 0)
    {
        write_to.copy_from(*current);
        return true;
    }
    // Otherwise, continue traversal, decrementing index
    else
    {
        return select_helper(index - 1, write_to, current->get_next());
    }
}

// Traverses the LLL, displaying each node until the end
void BattleHistoryList::print_helper(const BattleHistoryNode *current, int index) const
{
    if(current != NULL)
    {
        current->print(index);
        print_helper(current->get_next_readonly(), ++index);
    }
}

// Traverses the entire node, saving each node until the end
void BattleHistoryList::save_helper(std::ofstream & ofs, BattleHistoryNode *current)
{
    if(current != NULL)
    {
        current->save(ofs);
        save_helper(ofs, current->get_next());
    }
}
/* ************************* */