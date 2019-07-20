/* -*- Mode: C; c-basic-offset:4 ; indent-tabs-mode:nil ; -*- */
/*
 *  (C) 2001 by Argonne National Laboratory.
 *      See COPYRIGHT in top-level directory.
 */

#include "mpi.h"
#include <iostream>
#include <string>
#include <cmath>
using namespace std;

#define HX_PARALLEL

double f(double a);

double f(double a)
{
    return (4.0 / (1.0 + a * a));
}

int HXInit();
int HXInit( int & argc, char *** argv );
void HXFinalize();
int HXRank();
int HXSize();

int HXInit()
{
    int err      = 0;
    int argc     = 0;
    char *** argv = 0;
    return HXInit( argc, argv );
}

int HXInit( int & argc, char *** argv )
{
    int err = 0;
#ifdef HX_PARALLEL
    err = MPI_Init( & argc, argv );
#endif
    return err;
}

void HXFinalize()
{
#ifdef HX_PARALLEL
    int err = MPI_Finalize();
#endif
}

int HXRank()
{
    int rank = 0;
#ifdef HX_PARALLEL
    MPI_Comm_rank( MPI_COMM_WORLD, & rank );
#endif
    return rank;
}

int HXSize()
{
    int size = 1;
#ifdef HX_PARALLEL
    MPI_Comm_size( MPI_COMM_WORLD, & size );
#endif
    return size;
}

std::string HXGetProcessorName()
{
    string procName = "";
#ifdef HX_PARALLEL
    char cName[ MPI_MAX_PROCESSOR_NAME ];
    int nLength = 0;

    MPI_Get_processor_name( cName, & nLength );
    procName = cName;
#endif
    return procName;
}


int main(int argc, char *argv[])
{
    int n, myid, numprocs, i;
    double PI25DT = 3.141592653589793238462643;
    //double mypi, pi, h, sum, x;
    double h, sum, x;
    double startwtime = 0.0, endwtime;
    int namelen;
    //char processor_name[MPI_MAX_PROCESSOR_NAME];

    cout << "--------------before haha----------------------\n";
    cout << "--------------HXInit()----------------------\n";
    HXInit();
    //HXInit(argc, &argv );
    //MPI_Init(&argc, &argv);
    //MPI_Comm_size(MPI_COMM_WORLD, &numprocs);
    //MPI_Comm_rank(MPI_COMM_WORLD, &myid);
    numprocs = HXSize();
    myid = HXRank();
    //MPI_Get_processor_name(processor_name, &namelen);
    string processor_name = HXGetProcessorName();



    cout << "haha\n";
    //cout << "Process " << myid << " of " << numprocs << " is on " << processor_name << "\n";
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

    double mypi = h * sum;
    double pi;

    MPI_Reduce(&mypi, &pi, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);
    double aaa = 1.0;
    double bbb = -1.0;
    MPI_Allreduce(&aaa, &bbb, 1, MPI_DOUBLE, MPI_SUM, MPI_COMM_WORLD);
    cout << " myid = " << myid << " aaa=" << aaa << " bbb = " << bbb << "\n";

    int a = 1;
    int b = 0;
    MPI_Allreduce( &a, &b, 1, MPI_INTEGER, MPI_SUM, MPI_COMM_WORLD );
    cout << "myid = " << myid << " a = " << a << " b = " << b << "\n";

    int c = 1;
    int d = -1;
    MPI_Allreduce( &c, &d, 1, MPI_INT, MPI_SUM, MPI_COMM_WORLD );
    cout << "myid = " << myid << " c = " << c << " d = " << d << "\n";

    if ( myid == 0 )
    {
        endwtime = MPI_Wtime();
        cout << "pi is approximately " << pi << " Error is " << fabs(pi - PI25DT) << "\n";
        cout << "wall clock time = " << endwtime - startwtime << "\n";
    }

    //MPI_Finalize();
    HXFinalize();
    return 0;
}
