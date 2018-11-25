/************************************************************************
  File:          worker.h                              
  Description:   Functions for the workers.
                                                         
  Author:        Dana Vrajitoru, IUSB 
  Course:        B424 B524 Parallel Programming                        
  Date:          November 2018
************************************************************************/

#ifndef WORKER_H
#define WORKER_H

#include "puzzle.h"

// Loops over waiting for a task and executing it until it receives a
// task from the master indicating that the job is finished.
void Worker(int proc_id);

// Send the master whatever it is that the master will simply
// discard. For example, 5 integer values, the first one being 0.
void Send_empty_result();

// Find the word in the puzzle, then send the master 5 integer values:
// the size of the word, the position where it was found, and the
// direction in which it was found. If the word is not found in the
// puzzle, then the worker must send the master a size of 0 in which
// case the master must not do anything about it (basically sending an
// empty result).
void Process_word(char *word, Puzzle &the_puzzle);

#endif
