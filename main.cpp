
#include "battle.h"
#include "characterhandler.h"

#include "functions.h"

/* FUNCTIONS FROM functions.h
 * *******************************
 */
// Initializes random generator, providing
// current time for seed
void initialize_program()
{
    srand(time(NULL));
}

// Retrieves input from user,
// writing into supplied value
void get_input(char *write_to)
{
    using namespace std;
    cin.width(READ_WIDTH); 

    cin.get(write_to, READ_WIDTH, '\n');

    // If too large an input was provided, inform
    // user of problem, and repeat retrieval
    while(cin.peek() != '\n')
    {
        cin.ignore(READ_WIDTH, '\n');
        cout << "You entered too large a value - please try again!" << endl;
        cin.get(write_to, READ_WIDTH, '\n');
    }
    cin.ignore(READ_WIDTH, '\n');
}

// Returns an integer input from user
int get_int_input(int max)
{
    using namespace std;

    cout << ">> ";
    char input[MAX_INT_INPUT]; 
    cin.width(MAX_INT_INPUT);

    // Attemps to convert user's input into an
    // integer
    while(true)
    {
        try
        {
            cin.get(input, MAX_INT_INPUT, '\n');
            cin.ignore(MAX_INT_INPUT, '\n');
            int input_int = -1;
            input_int = stoi(input);

            // If number falls within range (1 to max)
            // return number
            if(input_int <= max && input_int >= 1)
            {
                return input_int;
            }
            // If not, repeat
            else
            {
                cout << "That number is not appropriate. Please try again." << endl;
                return get_int_input(max); 
            }
        }
        // If number is not provided by user, inform them so, and repeat process
        catch(invalid_argument) 
        { 
            cout << "That is not a valid input. Please try again." << endl;
            return get_int_input(max); 
        }
    }
}

// Clears the terminal
// works for Linux and Windows OS's
void clear_screen()
{
    if(!DEBUG)
    {
        #ifdef _WIN32
            system("cls");
        #else
            system("clear");
        #endif
    }
}

// Waits for user to press ENTER
void wait_for_enter()
{
    using namespace std;
    cout << "Press ENTER to continue..." << endl;
    cin.ignore(MAX_INT_INPUT, '\n');
}

// Creates a random integer, from minmum (inclusive),
// to maximum (exclusive)
int rand(int min, int max)
{
    return (random() % (max - min)) + min;
}

// Creates a random integer, from 0 (inclusive),
// to maximum (exclusive)
int rand(int max)
{
    return rand(0, max);
}

// Rolls a number between 1 and 10
int roll_10() { return rand(1, 11); }

// Converts an integer to a non-negative value
// if number is negative, return 0
int at_least_0(int val)
{
    if(val < 0) return 0;
    return val;
}
/* ********************************
 */



/* MAIN
 * ************************
 */
int main()
{
    initialize_program();

    // Create the CharacterHandler and begin the UI loop
    CharacterHandler handler = CharacterHandler();
    handler.ui_loop();
}
/* *************************
 */