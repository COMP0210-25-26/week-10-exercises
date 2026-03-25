#include <cstdio>
#include "RubbishHash.h"
#include <mpi.h>
#include <vector>

// Using an enum for MPI tags is great for clarifying MPI calls 
enum MpiTags
{
    result
};

int main()
{
    // MPI setup boiler-plate
    MPI_Init(NULL, NULL);
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    int n_proc;
    MPI_Comm_size(MPI_COMM_WORLD, &n_proc);

    unsigned long hash = 504981L;

    const int N = 10000;

    // determining the sub-domain for each process based on their rank
    unsigned int x_min = N / n_proc * rank;
    unsigned int x_max = (N / n_proc) * (rank + 1);
    if(rank == (n_proc - 1))
    {
        x_max = N;
    }

    unsigned int key = 0;

    // setting up data for the results
    // strictly speaking only rank 0 needs an array for found, and the others only need a bool
    // we have left this on the other ranks to make the code simpler and more streamlined
    bool solved = false;
    bool* found = new bool[n_proc];

    for(int i = 0; i < n_proc; i++)
    {
        found[i] = false;
    }

    for(unsigned int i = x_min; i < x_max; i++)
    {

        // every process tests one input to try to find a solution and records success if it does
        if(rubbish_hash(i) == hash)
        {
            found[rank] = true;
            key = i;
        }
        
        // every iteration we are synchronously gathering results from all processes
        // this array tells us if any of the processes found a match
        MPI_Gather(&found[rank], 1, MPI_C_BOOL, found, 1, MPI_C_BOOL, 0, MPI_COMM_WORLD);

        // Only rank 0, which has gathered all results, checks the array for any successes
        if(rank == 0)
        {
            for (int r = 0; r < n_proc; r++)
            {
                if (found[r])
                {
                    solved = true;
                }
            }
        }

        // Rank 0 then tells all other processes whether they can stop looking
        MPI_Bcast(&solved, 1, MPI_C_BOOL, 0, MPI_COMM_WORLD);
        if(solved) break;
    }

    // If a process other than rank 0 found the result, that result needs to be communicated
    // Note that this solution does address what would happen if multiple processes found a result
    if (rank == 0)
    {
        std::vector<int> result;  
        std::vector<int> winners;

        if(found[0])
        {
            printf("Rank 0 found x = %u\n", key);
        }
        for(int i = 1; i < n_proc; i++)
        {
            // if rank i found a result, receive it and add it to the results list
            if(found[i])
            {
                MPI_Recv(&key, 1, MPI_UNSIGNED, i, MpiTags::result, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
                printf("Rank 0 was told that %d found x = %u\n", i, key);
            }
        }        
    }
    else if ((rank != 0) && (found[rank] == true))
    {
        // Every process with rank != 0 that found a result should send a message
        // this is matched by the receives in the loop on rank 0.
        MPI_Ssend(&key, 1, MPI_UNSIGNED, 0, MpiTags::result, MPI_COMM_WORLD);
    }

    // At this point all communications should have completed and no ranks should be deadlocked/hanging
    printf("Rank %d came to a graceful stop.\n", rank);


    delete[] found;  // clean up! 

    MPI_Finalize();

    return 0;
}