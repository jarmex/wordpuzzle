/************************************************************************
  File:          puzzle.h
  Description:   A program that solves a word find puzzle.
                 Declaration of a class handling the puzzle and the 
                 solving functions.
                                                         
  Author:        Dana Vrajitoru, IUSB 
  Course:        B424 B524 Parallel Programming                        
  Date:          November 2018
************************************************************************/

#ifndef PUZZLE_H
#define PUZZLE_H

#include <fstream>
using namespace std;

class Puzzle {
private:
    char *grid;
    int rows, cols;
    
public:
    // Constructor with number of rows and columns.
    Puzzle(int nrows=0, int ncols=0);
    // Allocate the memory to store the grid.
    void Init(int nrows=0, int ncols=0);
    // Input the grid from a file.
    bool Read(ifstream &fin);
    
    // Send the grid to all the slaves
    void Broadcast(int source, int proc_id);
    
    // Checks if [i,j] is a safe position in the array
    bool issafe(int i, int j);
    
    // Subscript operator to access an entire row.
    char *operator[](int i);
    
    // Print the puzzle
    void Print();
    
    // Checks if the given word can be found starting from the cell
    // (posr, posc) in the directions given by the second and third
    // parameters. rdir applies to the rows, for example -1 goes
    // backward, +1 goes forward, 0 means constant row. Same rule
    // applies to the columns.
    bool Check_word(char *word, int posr, int posc, int rdir, int cdir);
    
    // Functions to be completed by the students.
    
    // Finding a word in the grid. It must return the position [rpos,
    // cpos] where it found the word and the directions in which it
    // found it. An example is given for horizontal forward and diagonal
    // backwards. The student must complete the search in 8 directions.
    bool Find_word(char *word, int &posr, int &posc, int &rdir, int &cdir);
    
    // This function should overwrite a word of the given size in the
    // grid by spaces, starting from the given position and following
    // the directions indicated by the appropriate parameters. We'll
    // assume that the word has been found at that position, but if we
    // get the function gets out of the grid because of an error
    // somewhere else in the program, the function should output an
    // error message.
    void Delete_word(int size, int posr, int posc, int rdir, int cdir);
    
    // We assume here that the master has deleted all of the letters
    // that belong to a word from the table, meaning that it has
    // overwriten them by spaces. This function should print the
    // remaining letters with just a space between them and with no
    // endlines except for one at the end, and also tell the user in the
    // beginning that this is the secret message or solution to the
    // puzzle.
    void Print_secret_message();
};

#endif
