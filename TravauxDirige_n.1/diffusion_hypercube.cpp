#include <cstdlib>
#include <sstream>
#include <string>
#include <fstream>
#include <iostream>
#include <iomanip>
#include <mpi.h>

int main( int nargs, char* argv[] ) {
  /* Init */
  MPI_Init( &nargs, &argv );
  MPI_Comm globComm;
  MPI_Comm_dup(MPI_COMM_WORLD, &globComm);
  int nbp;
  MPI_Comm_size(globComm, &nbp);
  int rank;
  MPI_Comm_rank(globComm, &rank);
  /* File */
  std::stringstream fileName;
  fileName << "Output" << std::setfill('0') << std::setw(5) << rank << ".txt";
  std::ofstream output( fileName.str().c_str() );


  output.close();
  MPI_Finalize();
  return EXIT_SUCCESS;
}
