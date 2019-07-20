#include <iostream>
#include <string>
#include <vector>
#include "mpi.h"
#include "metis.h"
using namespace std;

#define HX_PARALLEL
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

void PartByMetis( idx_t nCell, vector<idx_t>& xadj, vector<idx_t>& adjncy )
{
    idx_t   ncon     = 1;
    idx_t   * vwgt   = 0;
    idx_t   * vsize  = 0;
    idx_t   * adjwgt = 0;
    float * tpwgts = 0;
    float * ubvec  = 0;
    idx_t options[ METIS_NOPTIONS ];
    idx_t wgtflag = 0;
    idx_t numflag = 0;
    idx_t objval;
    idx_t nZone = 2;

    vector<idx_t> gc2lzone;

    METIS_SetDefaultOptions( options );
    cout << "Now begining partition graph!\n";
    if ( nZone > 8 )
    {
        cout << "Using K-way Partitioning!\n";
        METIS_PartGraphKway( & nCell, & ncon, & xadj[ 0 ], & adjncy[ 0 ], vwgt, vsize, adjwgt, 
                             & nZone, tpwgts, ubvec, options, & objval, & gc2lzone[ 0 ] );
    }
    else
    {
        cout << "Using Recursive Partitioning!\n";
        METIS_PartGraphRecursive( & nCell, & ncon, & xadj[ 0 ], & adjncy[ 0 ], vwgt, vsize, adjwgt, 
                                  & nZone, tpwgts, ubvec, options, & objval, & gc2lzone[ 0 ] );
    }
    cout << "The interface number: " << objval << endl; 
    cout << "Partition is finished!\n";
}

int main( int argc, char ** argv )
{
    //cout << "haha\n";
    //HXInit();
    //string s = HXGetProcessorName();
    //cout << " The processor name is  " << s << "\n";
    //HXFinalize();
    int myid, numprocess;
    int namelen;
    char processor_name[MPI_MAX_PROCESSOR_NAME];
    cout << "haha\n";
    cout << " argc = " << argc << "\n";
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &myid);
    MPI_Comm_size(MPI_COMM_WORLD, &numprocess);
    MPI_Get_processor_name(processor_name, &namelen);

    cout << "To you : Process " << myid << "of" << numprocess << " on " << processor_name << "\n";
    int n = 1;
    int m = 0;
    //MPI_Allreduce( &n, &m, 1, MPI_INTEGER, MPI_SUM, MPI_COMM_WORLD );
    MPI_Allreduce( &n, &m, 1, MPI_INT, MPI_SUM, MPI_COMM_WORLD );
    cout << "myid = " << myid << " n = " << n << " m = " << m << "\n";

    MPI_Finalize();

    return 0;
}