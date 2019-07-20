/* -*- Mode: C; c-basic-offset:4 ; indent-tabs-mode:nil ; -*- */
/*
 *  (C) 2001 by Argonne National Laboratory.
 *      See COPYRIGHT in top-level directory.
 */

#include "mpi.h"
#include <iostream>
#include <cmath>
using namespace std;

double f(double a);

double f(double a)
{
    return (4.0 / (1.0 + a * a));
}

int main(int argc, char *argv[])
{
    int n, myid, numprocs, i;
    double PI25DT = 3.141592653589793238462643;
    double mypi, pi, h, sum, x;
    double startwtime = 0.0, endwtime;
    int namelen;
    char processor_name[MPI_MAX_PROCESSOR_NAME];

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &numprocs);
    MPI_Comm_rank(MPI_COMM_WORLD, &myid);
    MPI_Get_processor_name(processor_name, &namelen);

    cout << "haha\n";
    cout << "Process " << myid << " of " << numprocs << " is on " << processor_name << "\n";

    n = 10000;  /* default # of rectangles */
    if ( myid == 0 )
    {
        startwtime = MPI_Wtime();
    }
        

    MPI_Bcast(&n, 1, MPI_INT, 0, MPI_COMM_WORLD);

    h = 1.0 / (double) n;
    sum = 0.0;
    /* A slightly better approach starts from large i and works back */
    for ( int i = myid + 1; i <= n; i += numprocs )
    {
        x = h * ((double) i - 0.5);
        sum += f(x);
    }
    mypi = h * sum;

    MPI_Reduce(&mypi, &pi, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);

    if ( myid == 0 )
    {
        endwtime = MPI_Wtime();
        cout << "pi is approximately " << pi << " Error is " << fabs(pi - PI25DT) << "\n";
        cout << "wall clock time = " << endwtime - startwtime << "\n";
    }

    MPI_Finalize();
    return 0;
}
