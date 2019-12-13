/*
 * Christian Schmid
 * CS202 - Programming Systems
 * Program 1 - battlehistory.h
 * Definition of BattleHistory, BattleHistoryNode,
 * and BattleHistory List class
 */ 

#ifndef BATTLEHISTORY_H_
#define BATTLEHISTORY_H_

#include <iostream>
#include <fstream>

// BattleHistory holds all information related to a Battle - what
// character was being fought, if the Battle was won, and how much
// experience was gained.
class BattleHistory
{
    public:
        // Constructors / Destructors
        BattleHistory();
        BattleHistory(const BattleHistory & source);
        BattleHistory(const bool did_win, const char * op_name, const int exp);
        ~BattleHistory();

        // Compares with a second BattleHistory, to see if they
        // match this one
        bool equals(const BattleHistory & history) const;

        // Prints the BattleHistory's information
        void print(const int offset) const;

        // Returns whether this BattleHistory is a win
        bool is_win() const;

        // Appends BattleHistory information to the
        // supplied ofstream
        void save(std::ofstream & ofs);

        // Copies contents of provided BattleHistory
        void copy_from(const BattleHistory & source);

    protected:
        bool win; // Is this BattleHistory a win?
        char *opponent_name; // Who was the opponent?
        int exp_gained; // How much experience did this Character gain?
};

// BattleHistoryNode is a BattleHistory that includes an LLL node structure
// used for implementation in a BattleHistoryList
class BattleHistoryNode : public BattleHistory
{
    public:
        BattleHistoryNode();
        BattleHistoryNode(const BattleHistoryNode & source);
        BattleHistoryNode(const BattleHistory & source);
        BattleHistoryNode(const bool did_win, const char * op_name, const int exp);
        ~BattleHistoryNode();

        // Retrieves the next BattleHistoryNode in the LLL
        BattleHistoryNode *& get_next();
        // Clears the next element
        void clear_next();
        // Retrieves the next node, but as a const, so it
        // cannot be edited
        const BattleHistoryNode *get_next_readonly() const;
        // Sets the next BattleHistoryNode in the LLL
        void set_next(BattleHistoryNode *& source);

    private:
        BattleHistoryNode *next;
};

// An LLL of BattleHistories, which provides storage,
// retrieval, and display functionality
class BattleHistoryList
{
    public:
        // Constructors/Destructor
        BattleHistoryList();
        BattleHistoryList(const BattleHistoryList & source);
        ~BattleHistoryList();

        // Returns the # of elements in the LLL
        int get_count() const;

        // Inserts a new BattleHistory into the LLL
        void insert(const BattleHistory & history);

        // Removes a BattleHistory from the LLL (by index)
        bool remove(const int index);

        // Select a BattleHistory from the list, by index
        bool select(const int index, BattleHistory & write_to);

        // Clears the entire BattleHistoryList of elements
        void clear();

        // Displays the BattleHistoryList
        void print() const;

        // Appends LLL to supplied ofstream
        void save(std::ofstream & ofs);

    private:
        // PRIVATE RECURSIVE HELPER METHODS
        void copy_list(const BattleHistoryNode *from, BattleHistoryNode *& to);
        void print_helper(const BattleHistoryNode *current, int index) const;
        void insert_helper(const BattleHistory & history, BattleHistoryNode *& current);
        bool remove_helper(const int index, BattleHistoryNode *& current);
        bool select_helper(const int index, BattleHistory & write_to, BattleHistoryNode *current) const;
        void save_helper(std::ofstream & ofs, BattleHistoryNode *current);

        BattleHistoryNode *head; // The first element in the LLL
        int count; // The # of elements in the LLL
};
#endif