/************************************************************************
  File:          master.h                         
  Description:   Functions for the master.
                                                         
  Author:        Dana Vrajitoru, IUSB 
  Course:        B424 B524 Parallel Programming                        
  Date:          November 2018
************************************************************************/

#ifndef MASTER_H
#define MASTER_H

#include "puzzle.h"

// The master that generates tasks and assigns them to worker
// processes.
void Master(int nr_proc);

// Sends all of the workers a stopping signal.
void Finish_all(int nr_workers, char *word, Puzzle &the_puzzle);

// The master must receive the result from a worker consisting of 5
// integers, one for the size of the word the worker has been looking
// for, two for the position where it was found, and two for the
// direction in which it was found. The master must then call the
// Delete method from the puzzle with these five parameters, and then
// return the identity of the worker. The receive must be called with
// MPI_ANY_SOURCE for the source. The identity of the worker can be
// found in status.MPI_SOURCE. You can either send/receive an array of
// 5 integers, of use 5 messages, but make sure that they come from
// the same process, meaning that after you get the id from the first
// message, you must use that as the source in the other receive
// messages. A size of 0 should mean that the word was not found in
// the grid. The function should return the identity of the process it
// has received the result from.
int Recv_result(Puzzle &the_puzzle);

#endif
