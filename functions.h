/*
 * Christian Schmid
 * CS202 - Programming Systems
 * Program 1 - functions.h
 * 
 * Definition of all helper functions for program 1.
 * Functions defined here do not have a parent class,
 * and serve more as extensions to the program.
 */

#ifndef FUNCTIONS_H_
#define FUNCTIONS_H_

#include<iostream>
#include<stdlib.h>
#include<time.h>

const bool DEBUG = false;

const int READ_WIDTH = 256;

const int MAX_INT_INPUT = 3;

// Initializes random generator, providing
// current time for seed
void initialize_program();

// Retrieves input from user,
// writing into supplied value
void get_input(char *write_to);

// Returns an integer input from user
int get_int_input(int max);

// Clears the terminal
// works for Linux and Windows OS's
void clear_screen();

// Waits for user to press ENTER
void wait_for_enter();

// Creates a random integer, from minmum (inclusive),
// to maximum (exclusive)
int rand(int min, int max);

// Creates a random integer, from 0 (inclusive),
// to maximum (exclusive)
int rand(int max);

// Rolls a number between 1 and 10
int roll_10();

// Converts an integer to a non-negative value
// if number is negative, return 0
int at_least_0(int val);

#endif
