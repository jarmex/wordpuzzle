/************************************************************************
  File:          puzzle.cc
  Description:   A program that solves a word find puzzle.
                 Implementation of a class handling the puzzle and the 
                 solving functions.
                                                         
  Author:        Dana Vrajitoru, IUSB 
  Course:        B424 B524 Parallel Programming                        
  Date:          November 2018
************************************************************************/

#include <iostream>
using namespace std;
#include <cstdlib>
#include <mpi.h>
#include "puzzle.h"

// Constructor with number of rows and columns.
Puzzle::Puzzle(int nrows, int ncols)
    : grid(NULL), rows(nrows), cols(ncols)
{
    // We need to allocate nrows x (nrcols+1) to be able to store
    // a '\0' character at the end and read a whole row of the
    // puzzle at a time.
    if (nrows && ncols)
        grid = new char[nrows * (ncols + 1)];
}

// Allocate the memory to store the grid.
void Puzzle::Init(int nrows, int ncols)
{
    if (grid)
        delete[] grid;

    if (nrows && ncols)
        grid = new char[nrows * (ncols + 1)];
    else
        grid = NULL;
}

// Input the grid from a file.
bool Puzzle::Read(ifstream &fin)
{
    int i;
    fin >> rows >> cols;
    if (rows && cols)
    {
        Init(rows, cols);
        for (i = 0; i < rows && fin.good() && !fin.eof(); i++)
        {
            fin >> (*this)[i];
            if (strlen((*this)[i]) != cols)
            {
                cout << "Missing characters on row " << i << endl;
                return false;
            }
        }
        return (i == rows);
    }
    else
        return true;
}

// Send the grid to all the slaves
void Puzzle::Broadcast(int source, int proc_id)
{
    MPI_Bcast(&rows, 1, MPI_INT, source, MPI_COMM_WORLD);
    MPI_Bcast(&cols, 1, MPI_INT, source, MPI_COMM_WORLD);
    if (proc_id != source)
        Init(rows, cols);
    MPI_Bcast(grid, rows * (cols + 1), MPI_CHAR, source, MPI_COMM_WORLD);
}

// Checks if [i,j] is a safe position in the array
bool Puzzle::issafe(int i, int j)
{
    return (0 <= i && 0 <= j && i < rows && j <= cols);
}

// Subscript operator to access an entire row.
char *Puzzle::operator[](int i)
{
    if (0 <= i && i < rows)
        return grid + i * (cols + 1);
    else
    {
        cout << "Illegal access to row: " << i << endl
             << "Number of rows: " << rows << endl;
        exit(1);
    }
}

// Print the puzzle
void Puzzle::Print()
{
    cout << "The puzzle of size " << rows << "x" << cols << " is:" << endl;
    for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j < cols; j++)
            cout << (*this)[i][j] << ' ';
        cout << endl;
    }
    cout << endl;
}

// Checks if the given word can be found starting from the cell (posr,
// posc) in the directions given by the second and third
// parameters. rdir applies to the rows, for example -1 goes backward,
// +1 goes forward, 0 means constant row. Same rule applies to the
// columns.
bool Puzzle::Check_word(char *word, int posr, int posc, int rdir, int cdir)
{
    bool found = true;
    int i, j, k, size = strlen(word);

    i = posr;
    j = posc;
    k = 0;
    while (k < size && issafe(i, j) && found)
    {
        if ((*this)[i][j] != word[k])
            found = false;
        else
        {
            k++;
            i = i + rdir;
            j = j + cdir;
        }
    }
    return (k == size);
}

// Functions to be completed by the students.

// Finding a word in the grid. It must return the position [rpos,
// cpos] where it found the word and the directions in which it found
// it. An example is given for horizontal forward and diagonal
// backwards. The student must complete the search in 8 directions.
bool Puzzle::Find_word(char *word, int &posr, int &posc, int &rdir, int &cdir)
{
    int ii, jj;
    for (posr = 0; posr < rows; posr++)
        for (posc = 0; posc < cols; posc++)
        {
            for (ii = -1; ii < 2; ii++)
            {
                for (jj = -1; jj < 2; jj++)
                {
                    rdir = ii;
                    cdir = jj;
                    if (Check_word(word, posr, posc, rdir, cdir))
                        return true;
                }
            }
        }
    return false;
}
// This function should overwrite a word of the given size in the grid
// by spaces, starting from the given position and following the
// directions indicated by the appropriate parameters. We'll assume
// that the word has been found at that position, but if the function
// gets out of the grid because of an error somewhere else in the
// program, the function should output an error message.
void Puzzle::Delete_word(int size, int posr, int posc, int rdir, int cdir)
{
    // Code to be supplied by the student.
    for (int i = 0; i < size; i++)
    {
        (*this)[posr + (i * rdir)][posc + (i * cdir)] = ' ';
    }
}

// We assume here that the master has deleted all of the letters that
// belong to a word from the table, meaning that it has overwriten
// them by spaces. This function should print the remaining letters
// with just a space between them and with no endlines except for one
// at the end, and also tell the user in the beginning that this is
// the secret message or solution to the puzzle.
void Puzzle::Print_secret_message()
{
    cout << "This is the secret message: " << endl; 
    for (int posr = 0; posr < rows; posr++){
        for (int posc = 0; posc < cols; posc++)
        {
            if ((*this)[posr][posc] != ' ')
            {
                cout << (*this)[posr][posc] << ' ';
            }
        }
    }
    cout << endl;
}
