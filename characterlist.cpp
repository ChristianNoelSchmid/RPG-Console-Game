#include "characterlist.h"
#include "characterhandler.h"

/* CHARACTERARRAY MEMBER FUNCTIONS (characterlist.h)
 * *************************
 */
// CharacterArray Default Constructor
CharacterArray::CharacterArray()
    : char_type(WARRIOR), array(NULL), count(0), capacity(4) 
{
    array = new Character*[capacity];
    for(int i = 0; i < capacity; ++i)
    {
        array[i] = NULL;
    }
}

// CharacterArray Copy Constructor
// Copies CharacterType, count, and capacity, as well
// as Characters themselves
CharacterArray::CharacterArray(const CharacterArray & source)
    : char_type(source.char_type), count(source.count), capacity(source.capacity)
{
    array = new Character*[capacity];
    for(int i = 0; i < capacity; ++i)
    {
        if(i < count)
        {
            array[i] = source.array[i]->clone();
        }
        else
        {
            array[i] = NULL;
        }
    } 
}

// CharacterArray Primary Constructor
// Provides CharacterType, which is used for successive
// insertions
CharacterArray::CharacterArray(CharacterType type)
    : char_type(type), array(NULL), count(0), capacity(4) 
{
    array = new Character*[capacity];
    for(int i = 0; i < capacity; ++i)
    {
        array[i] = NULL;
    }
}


// CharacterArray Destructor
CharacterArray::~CharacterArray()
{
    if(array != NULL)
    {
        for(int i = 0; i < count; ++i)
        {
            delete (array[i]);
        }
        delete [] array;
    }
}

// Tests whether Array contains supplied CharacterType
bool CharacterArray::matches_char_type(const CharacterType type) const 
{   return char_type == type; }

// Inserts Character into CharacterArray
void CharacterArray::insert(const Character & character)
{
    // If capacity is reached
    if(count == capacity)
    {
        // Multiply capacity by 2, and create a new
        // array of that capacity
        capacity *= 2;
        Character **temp = new Character*[capacity];

        // Copy all elements from array to new
        // one. Set all other elements in the array
        // to NULL
        for(int i = 0; i < capacity; ++i)
        {
            if(i < count)
            {
                temp[i] = array[i];
                array[i] = NULL;
            }
            else
            {
                temp[i] = NULL;
            }
        } 

        // Delete array and reassign to new one
        delete [] array;
        array = temp;
    }

    // Add new element to array, and increase
    // count
    array[count] = character.clone();
    ++count;
}


// Removes Character from CharacterArray, by index
bool CharacterArray::remove(const int index)
{
    // If index is within bounds of elements
    // proceed with removal
    if(index >= 0 && index < count)
    {
        // Delete the specified element at index
        delete (array[index]);

        // For every successive element, move pointer
        // down, until end of list is reached
        for(int i = index + 1; i < count; ++i)
        {
            array[i - 1] = array[i];
            if(i == count - 1)
            {
                array[i] = NULL;
            }
        }

        // Subtract count
        --count;
        return true;
    }
    // If not, return false - element wasn't removed
    return false;
}

// Display's whole array of Characters
void CharacterArray::print(const bool print_ind, int offset_index) const
{
    // Determine what class CharacterArray is for,
    // and print appropriate header
    if(char_type == WARRIOR) 
    {
        std::cout << "Warrior\n--\n";
    }
    else if(char_type == RANGER)
    {
        std::cout << "Ranger\n--\n";
    }
    else
    {
        std::cout << "Magician\n--\n";
    }
    
    // Print each element in the array.
    // If print_ind, display the element's index
    // (Dependent on container CharacterList)
    for(int i = 0; i < count; ++i)
    {
        std::cout << "  ";
        if(print_ind) 
        {
            std::cout << i + offset_index + 1 << ") ";
        }
        array[i]->print(true);
        std::cout << std::endl;
    }
    std::cout << std::endl;
}

// Gets total # of Characters in array
int CharacterArray::get_count() const { return count; }

// Selects Character by index (shallow copy)
Character *CharacterArray::select(int index) const
{
    // Return Character* if index is valid,
    // NULL if not
    if(index >= 0 && index < count)
        return array[index];

    return NULL;
}
/* ***********************
 */



/* CHARACTERNODE MEMBER FUNCTIONS (characterlist.h)
 * ************************
 */
// CharacterNode Default Constructor
CharacterNode::CharacterNode() : next(NULL) { }

// CharacterNode Copy Constructor
CharacterNode::CharacterNode(const CharacterNode & source)
    : CharacterArray(source), next(NULL)
{ }

// CharacterNode Primary Constructor - calls CharacterArray Constructor
// that assigns CharacterType
CharacterNode::CharacterNode(CharacterType type) : CharacterArray(type), next(NULL) { }

// CharacterNode Destructor
CharacterNode::~CharacterNode() 
{ 
    if(next != NULL)
    {
        delete next; 
    }
}


// Get's the next node in the CLL (by alias)
CharacterNode *& CharacterNode::get_next() { return next; }

// Get's the next node, but as readonly - not editable
const CharacterNode *CharacterNode::get_next_readonly() const { return next; }

// Sets the next node in the list (by alias)
void CharacterNode::set_next(CharacterNode *& val) { next = val; }

// Clears the next node in the list
void CharacterNode::clear_next()
{
    next = NULL;
}
/* *******************************
 */



/* CHARACTERLIST MEMBER FUNCTIONS (characterlist.h)
 * ********************************
 */
// CharacterList Default Constructor
CharacterList::CharacterList() : tail(NULL), count(0) { }

// CharacterList Copy Constructor
// Invokes copy_from to copy entire supplied CLL
CharacterList::CharacterList(const CharacterList & source)
    : count(0)
{
    if(source.tail == NULL) 
    {
        tail = NULL;
    }
    else
    {
        copy_from(source.tail, tail, source.get_count());  
    }
}

// CharacterList Destructor
CharacterList::~CharacterList()
{
    clear_list();
    tail = NULL;
}

// Returns total count of CharacterList
// including each element in each CharacterArray
int CharacterList::get_count() const
{
    return count;
}

// Inserts a new Character into the CharacterList
void CharacterList::insert(const Character & source)
{
    // If tail is NULL, set to source, and set it's next
    // to itself (CLL)
    if(tail == NULL)
    {
        tail = new CharacterNode(source.character_type());
        tail->insert(source);
        tail->set_next(tail);
    } 
    // If tail is not NULL, run insert_helper()
    else
    {
        insert_helper(source, tail->get_next());
    }
    
    ++count;
}

// Removes a Character, by index, from the CharacterList
// invokes remove_helper()
bool CharacterList::remove(const int index)
{
    return remove_helper(index, tail);     
}

// Clears all CharacterNodes from the CLL
void CharacterList::clear_list()
{
    if(tail != NULL)
    {
        // Grab head pointer, disconnect tail
        // from head, and delete head, clearing
        // the CLL recursively
        CharacterNode *head = tail->get_next();

        tail->clear_next();
        delete head;

        tail = NULL;
        count = 0;
    }
}

// Selects a Character from the list, by index
// invokes select_helper()
Character *CharacterList::select(const int index)
{
    return select_helper(tail->get_next(), index);
}

// Displays the whole CharacterList
// Invokes print_helper()
void CharacterList::print(const bool print_ind) const
{
    if(tail != NULL)
    {
        print_helper(tail->get_next(), print_ind, 0); 
    }
}

// Appends all data to the supplied ofstream
void CharacterList::save(std::ofstream & ofs)
{
    for(int i = 0; i < count; ++i)
    {
        select(i)->save(ofs);
        ofs << "\n";
    } 
}

// Traverses the CLL, copying each node until this CharacterList's count
// equals the copied count
void CharacterList::copy_from(const CharacterNode *from, CharacterNode *& to, const int ct)
{
    // Copy the node
    to = new CharacterNode(*from);

    // Increase count by count in CharacterArray
    count += from->get_count();

    // If count is reached, attach last added node to
    // tail
    if(count == ct)
    {
        to->set_next(tail);
    }
    // If not, continue traversal
    else
    {
        copy_from(from->get_next_readonly(), to->get_next(), ct);
    }
}

// Traverse the CLL, printing each node until until tail is reached
void CharacterList::print_helper(CharacterNode *& current, const bool print_ind, const int offset) const
{
    current->print(print_ind, offset);
    if(current != tail)
    {
        print_helper(current->get_next(), print_ind, offset + current->get_count());
    }
}

// Traverse the CLL, checking if node matches source CharacterType
// If it does, insert. If it doesn't exist, create a new node
void CharacterList::insert_helper(const Character & source, CharacterNode *& current)
{
    // If current's CharacterType matches source's, insert source
    if(current->matches_char_type(source.character_type()))
    {
        current->insert(source); 
    }
    // If tail has been reached,
    // create a new node and reassign tail
    else if(current == tail)
    {
        CharacterNode *head = tail->get_next();
        tail = new CharacterNode(source.character_type());
        tail->insert(source);

        current->set_next(tail);
        tail->set_next(head);
    }
    // Otherwise, continue traversal
    else
    {
        insert_helper(source, current->get_next());
    }
    
}

// Traverses CLL until specified offset is reached.
// Every traversal, the node's count is removed from offset, until
// CharacterNode with count that falls within offset is reached
// Traversal access' current's next, rather than current, to maitain
// pointer aliasing
bool CharacterList::remove_helper(const int offset, CharacterNode *& current)
{
    // Get next node in CLL
    CharacterNode *next = current->get_next();

    // If offset falls within next's bounds
    if(offset < next->get_count())
    {
        // Remove the index from next
        next->remove(offset);

        // If next no longer has any elements in
        // its array
        if(next->get_count() == 0)
        {
            // Simply delete the whole list if
            // it was the only node in the list
            if(count == 1)
            {
                clear_list();
            }

            // Otherwise, disconnect next from the 
            // CLL, reconnect previous node to next's next node
            // and delete next
            else
            {
                current->set_next(next->get_next());
                if(tail == next)
                {
                    tail = current;
                }

                next->clear_next();
                delete next;
            }
        }

        --count;
        return true;
    } 
    // If bounds still aren't reached at end of
    // traversal, return false
    else if(next == tail)
    {
        return false;
    }
    // Otherwise, continue traversal
    else
    {
        return remove_helper(offset - next->get_count(), next);
    }
}

// Traverses the list until specified offset is reached (see remove_helper())
// returns NULL if not found
Character *CharacterList::select_helper(CharacterNode *& current, int offset)
{
    if(offset < current->get_count())
    {
        return current->select(offset);
    }
    else if(current == tail)
    {
        return NULL;
    }
    else
    {
        return select_helper(current->get_next(), offset - current->get_count());
    }
}
/* **********************************
 */



/* CHARACTERHANDLER MEMBER FUNCTIONS (characterhandler.h)
 * ***************************
 */
// CharacterHandler Default and Primary Constructor
// loads list from file, if it exists
CharacterHandler::CharacterHandler()
{
    load_list();
}

// CharacterHandler Copy Constructor
CharacterHandler::CharacterHandler(const CharacterHandler & source)
    : list(source.list) { }

// CharacterHandler Destructor
CharacterHandler::~CharacterHandler()
{
    save_characters();
}

// Primary UI loop that provides the user
// with a options to add, remove, display, or
// battle Characters
void CharacterHandler::ui_loop()
{
    using namespace std;

    int input = -1;
    // Prompt User with options and selection
    while(input != 6)
    {
        clear_screen();

        cout << "Characters: " << endl;
        list.print(false);
        cout << "--" << endl << endl;

        cout << "Select an option (1-6): " << endl;
        cout << "  1) Add a Character" << endl;
        cout << "  2) Display Character" << endl;
        cout << "--" << endl;
        cout << "  3) Remove a Character" << endl;
        cout << "  4) Remove all Characters" << endl;
        cout << "--" << endl;
        cout << "  5) Battle Characters" << endl;
        cout << "--" << endl;
        cout << "  6) Exit" << endl << "--" << endl;
        input = get_int_input(6);

        // Call appropriate function based on input
        if(input == 1) { add_char(); }
        else if(input == 2) { display_char(); }
        else if(input == 3) { remove_char(); }
        else if(input == 4) { remove_all_chars(); }
        else if(input == 5) { battle_chars(); }
    }

    cout << "Goodbye!" << endl;
}

// UI that directs user to create new Character
void CharacterHandler::add_char()
{
    using namespace std;

    // Prompt User for new Character name
    char name[READ_WIDTH];
    cout << "Please enter a name for your character: ";
    get_input(name);

    // Prompt User to provide Character class
    cout << "Choose a class for " << name << ": " << endl;
    cout << "1) Warrior" << endl;
    cout << "2) Ranger" << endl;
    cout << "3) Magician" << endl;

    int class_idx = get_int_input(3);

    // Create specified Character from response
    if(class_idx == 1)
    {
        list.insert(Warrior(name));
    }
    else if(class_idx == 2)
    {
        list.insert(Ranger(name));
    }
    else
    {
        list.insert(Magician(name));
    }

    cout << name << " has been added!" << endl;
    wait_for_enter();
}

// UI that directs user to remove Character
void CharacterHandler::remove_char()
{
    clear_screen();

    using namespace std;

    // If there are no Characters in list, tell
    // user so, and return
    if(list.get_count() == 0)
    {
        cout << "There are no characters to remove!" << endl;
        wait_for_enter();
        return;
    }

    // Prompt user to select Character to remove
    cout << "Select a character to remove: " << endl;
    list.print(true);
    cout << "--" << endl;

    int input = get_int_input(list.get_count());

    // Confirm user wishes to delete Character
    cout << "Are you sure you wish to remove ";
    list.select(input - 1)->print(true);
    cout << "? (Y/N)" << endl << "  >> ";

    char choice; cin >> choice;
    cin.ignore(READ_WIDTH, '\n');

    // Remove, if yes
    if(tolower(choice) == 'y')
    {
        list.remove(input - 1);
        cout << "Character removed" << endl;
        wait_for_enter();
    }
}

// UI that directs user to display Character
void CharacterHandler::display_char()
{
    using namespace std;

    clear_screen();

    // If there are no Characters in list, inform user
    // and return
    if(list.get_count() == 0)
    {
        cout << "There are no characters to display!" << endl;
        wait_for_enter();
        return;
    }

    clear_screen();

    // Prompt user for choice of Character to display
    // Select that Character
    cout << "Select a character to display: " << endl;
    list.print(true);
    cout << "--" << endl;

    int input = get_int_input(list.get_count());
    Character *character = list.select(input - 1);

    // Print the Character's full Stats
    clear_screen();
    character->print(false);
    wait_for_enter();
}

// UI that directs user to remove all Characters
void CharacterHandler::remove_all_chars()
{
    clear_screen();
    using namespace std;

    // If there are no Characters in the list, inform
    // user, and return
    if(list.get_count() == 0)
    {
        cout << "There are no characters to remove!" << endl;
        wait_for_enter();
        return;
    }

    // Confirm with user that they wish to remove entire list
    cout << "Are you sure you wish to remove all characters? (Y/N)";
    cout << endl << "  >> ";
    char input; cin >> input;
    cin.ignore(READ_WIDTH, '\n');

    // If yes, clear list
    if(tolower(input) == 'y')
    {
        list.clear_list();
        cout << "All characters removed." << endl;
        wait_for_enter();
    }
}

// UI that directs user to battle Characters
void CharacterHandler::battle_chars()
{
    using namespace std;

    // Create a temporary CharacterList, which will remove
    // player 1's selected Character (to avoid Characters
    // fighting themselves!)
    CharacterList temp_list = CharacterList(list);
    int pl1 = 0, pl2 = 0;

    clear_screen();

    // If list includes less than 2 Characters, inform
    // user that 2 Characters are required, and return
    if(list.get_count() < 2)
    {
        cout << "You must have at least 2 character to battle!" << endl;
        wait_for_enter();
        return;
    }

    // Prompt for player 1 Character selection 
    temp_list.print(true);
    cout << "--" << endl;
    cout << "Please choose a character for player 1" << endl;

    pl1 = get_int_input(temp_list.get_count());
    temp_list.remove(pl1 - 1);
    clear_screen();

    // Prompt for player 2 Character selection 
    temp_list.print(true);
    cout << "--" << endl;
    cout << "Please choose a character for player 2" << endl;
    pl2 = get_int_input(temp_list.get_count());

    // If player 2's index selection is gte player 1, increase
    // player 2's index, because player 1's Character was removed from
    // list, and there's need for an offset
    if(pl2 >= pl1) { ++pl2; }

    // Grab both players from original list
    // and assign them to a new Battle
    Character *p1 = list.select(pl1 - 1);
    Character *p2 = list.select(pl2 - 1);

    Battle battle = Battle(*p1, *p2);
    int victor = battle.simulate();

    // Give appropriate experience to players
    int p1exp, p2exp;

    // If both players died, half experience for both
    if(victor == 2)
    {
        p1exp = p2->get_exp_value() / 2;
        p2exp = p1->get_exp_value() / 2;

        p1->insert_loss(*p2, p1exp);
        p2->insert_loss(*p1, p2exp);
    }

    // If p2 won, half for p1, and full for p2
    else if(victor == 1)
    {
        p2exp = p1->get_exp_value();
        p1exp = p2->get_exp_value() / 2;

        p1->insert_loss(*p2, p1exp);
        p2->insert_win(*p1, p2exp);
    }

    // If p1 won, half for p2, and full for p1
    else
    {
        p1exp = p2->get_exp_value();
        p2exp = p1->get_exp_value() / 2;

        p1->insert_win(*p2, p1exp);
        p2->insert_loss(*p1, p2exp);
    }

    p1->add_exp(p1exp);
    p2->add_exp(p2exp);

    p1->print(true); cout << " recieved " << p1exp << " experience." << endl;
    p2->print(true); cout << " recieved " << p2exp << " experience." << endl;
    wait_for_enter();
}

// Initializes loading procedure
void CharacterHandler::load_list()
{
    // Open file "data.dat", if it exists
    using namespace std;
    ifstream ifs; ifs.open("data.dat");
    ifs.peek();

    // If file exists, and it contains data
    // begin load_character procedure, calling
    // it until eof is reached
    while(ifs && !ifs.eof())
    {
        load_character(ifs);

        // Ignore '\n' after each Character is loaded to
        // goto next line
        ifs.ignore(READ_WIDTH, '\n');

        // Peek, so the eof flag will trigger if file is
        // read through
        ifs.peek();
    }

    // Always close the file!
    ifs.close();
}

// Loads Characters from file, if it exists
void CharacterHandler::load_character(std::ifstream & ifs)
{
    char temp[READ_WIDTH]; 
    char name[READ_WIDTH];
    int exp; CharacterType char_type;

    Character *character;

    // read first int and convert to CharacterType
    ifs.get(temp, READ_WIDTH, '|');
    ifs.ignore(READ_WIDTH, '|');
    char_type = static_cast<CharacterType>(atoi(temp));

    // read name
    ifs.get(name, READ_WIDTH, '|');
    ifs.ignore(READ_WIDTH, '|');

    // read experience
    ifs.get(temp, READ_WIDTH, '|');
    ifs.ignore(READ_WIDTH, '|');
    exp = atoi(temp);

    // Create correct derived Character, based on loaded
    // CharacterType
    if(char_type == WARRIOR) { character = new Warrior(name, exp); }
    else if(char_type == RANGER) { character = new Ranger(name, exp); } 
    else { character = new Magician(name, exp); }

    // If end-of-line is not reached, BattleHistory exists - call
    // load function
    if(ifs.peek() != '\n')
    {
        character->load_battle_history(ifs);
    }

    // Finally, insert new Character, cloning it
    // and deleting local Character
    list.insert(*character);
    delete character;
}


// Saves Characters to file
void CharacterHandler::save_characters()
{
    // Open file data.dat
    using namespace std;  
    ofstream ofs; ofs.open("data.dat");

    // If opening file was successful
    if(ofs)
    {
        // Save CharacterList
        list.save(ofs);
    }

    // Always remember to close the file!
    ofs.close();
}

/* ****************************************
 */