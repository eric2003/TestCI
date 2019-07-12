#include <iostream>
#include <string>
#include "mpi.h"
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

int main( int argc, char ** argv )
{
	cout << "haha\n";
	HXInit();
	string s = HXGetProcessorName();
	cout << " s= " << s << "\n";
	HXFinalize();
	return 0;
}