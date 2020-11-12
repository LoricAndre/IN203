#include <iostream>
#include <cstdlib>
#include <mpi.h>
#include <ctime>

int main( int nargs, char* argv[] ) {
  /* Init */
  MPI_Init( &nargs, &argv );
  MPI_Comm globComm;
  MPI_Comm_dup(MPI_COMM_WORLD, &globComm);
  int nbp;
  MPI_Comm_size(globComm, &nbp);
  int rank;
  MPI_Comm_rank(globComm, &rank);
  MPI_Status status;

  /* Tasks */
  int prev_i;
  if (rank == 0) {
    prev_i = 0;
  } else {
    MPI_Recv(&prev_i, 1, MPI_INT, rank-1, 0, MPI_COMM_WORLD, &status);
    prev_i++;
    std::cout << rank << " says " << prev_i << std::endl;
  }
  std::srand(std::time(nullptr) + rank);
  int i;
  i = std::rand() % 42;
  MPI_Send(&i, 1, MPI_INT, (rank+1)%nbp, 0, MPI_COMM_WORLD);

  /* Exit properly */
  MPI_Finalize();
  return EXIT_SUCCESS;
}
