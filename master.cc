/************************************************************************
  File:          master.cc                            
  Description:   A program that solves a word find puzzle.

                 Functions for the master. 
                                                         
  Author:        Dana Vrajitoru, IUSB 
  Course:        B424 B524 Parallel Programming                        
  Date:          November 2018
************************************************************************/

#include <iostream>
#include <fstream>
using namespace std;
#include <mpi.h>
#include <cstdlib>
#include <ctype.h>
#include "master.h"
#include "common_decl.h"
#include "puzzle.h"

// The master function. The master reads the puzzle, broadcasts it to
// the workers, then reads one word at a time and sends it to a worker
// to solve. Then it gets the result back from the workers and deletes
// the word from the puzzle. At the end it prints the secret message.

// The master that generates tasks and assigns them to worker
// processes.
void Master(int nr_proc)
{
    int nr_tasks, task, nr_workers = nr_proc - 1, worker;
    MPI_Status status;
    char puzzle_name[80];
    char word[WORDSIZE];
    
    // get the file name from the user, then input the puzzle from it
    // and broadcast it.
    cout << "Enter the name of the file containing the puzzle" << endl;
    cin >> puzzle_name;
    ifstream fin(puzzle_name);
    Puzzle the_puzzle;
    the_puzzle.Read(fin);
    the_puzzle.Print();
    the_puzzle.Broadcast(MASTER_ID, MASTER_ID);

    double time = MPI_Wtime();

    do
    {
        fin >> word;
        if (!fin.eof())
        {
            worker = Recv_result(the_puzzle);
            MPI_Send(word, WORDSIZE, MPI_CHAR, worker, 0, MPI_COMM_WORLD);
        }
    } while (!fin.eof());
    fin.close();

    Finish_all(nr_workers, word, the_puzzle);

    the_puzzle.Print_secret_message();

    time = MPI_Wtime() - time;
    cout << "This program has run on " << nr_proc
         << " processes in " << time << " seconds." << endl;
}

// Sends all of the workers a stopping signal.
void Finish_all(int nr_workers, char *word, Puzzle &the_puzzle)
{
    int worker;
    word[0] = '\0'; // An empty string.
    MPI_Status status;
    for (int i = 1; i <= nr_workers; i++)
    {
        worker = Recv_result(the_puzzle);
        MPI_Send(word, WORDSIZE, MPI_CHAR, worker, 0, MPI_COMM_WORLD);
    }
}

// The master must receive the result from a worker consisting of 5
// integers, one for the size of the word the worker has been looking
// for, two for the position where it was found, and two for the
// direction in which it was found. The master must then call the
// Delete method from the puzzle with these five parameters, and then
// return the identity of the worker. The receive must be called with
// MPI_ANY_SOURCE for the source. The identity of the worker can be
// found in status.MPI_SOURCE. You can either send/receive an array of
// 5 integers, or use 5 messages, but make sure that they come from
// the same process, meaning that after you get the id from the first
// message, you must use that as the source in the other receive
// messages. A size of 0 should mean that the word was not found in
// the grid. The function should return the identity of the process it
// has received the result from.
int Recv_result(Puzzle &the_puzzle)
{
    MPI_Status status;
    int receivedData[5]; // array format size, row, column, row_direction, column_direction;
    MPI_Recv(receivedData, 5, MPI_INT, MPI_ANY_SOURCE, 0, MPI_COMM_WORLD, &status);
    // check if the size of the word received is zero
    if (receivedData[0] > 0)
    {
        the_puzzle.Delete_word(
            receivedData[0],  // size
            receivedData[1],  // row
            receivedData[2],  // column
            receivedData[3],  // row_direction
            receivedData[4]); // column_direction
    }
    return status.MPI_SOURCE;
    // return 0; // placed here to avoid error
}
