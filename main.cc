/**********************************************************
  File:          main.cc                              
  Description:   A program for solving the word find puzzle
                 using the simple pool of tasks model.
                                                         
  Author:        Dana Vrajitoru, IUSB
  Course:        B424 B524 Parallel Programming 
  Date:          November 2018
***********************************************************/

#include <mpi.h>
#include <cstdlib>
#include "worker.h"
#include "master.h"
#include "common_decl.h"

//  The main function

int main(int argc, char **argv)
{
    int proc_id, nr_proc, n;

    /* Initialize MPI */
    MPI_Init (&argc, &argv);
    
    /* Get the rank of this process */
    MPI_Comm_rank (MPI_COMM_WORLD, &proc_id);
    
    /* Get nb of processes */
    MPI_Comm_size (MPI_COMM_WORLD, &nr_proc);
    
    if (proc_id == MASTER_ID)
        Master(nr_proc);
    else
        Worker(proc_id);

    /* Finalize MPI */
    MPI_Finalize ();
    return 0;
}
