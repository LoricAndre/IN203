# include <iostream>
# include <cstdlib>
# include <mpi.h>

int main( int nargs, char* argv[] ) {
  /* Initialisation */
  MPI_Init( &nargs, &argv );
  MPI_Comm globComm;
  MPI_Comm_dup(MPI_COMM_WORLD, &globComm);
  int nbp;
  MPI_Comm_size(globComm, &nbp);
  int rank;
  MPI_Comm_rank(globComm, &rank);

  std::cout << "Bonjour, je suis la tâche n° " << rank << " sur " << nbp << " tâches.\n";

  /* Exit properly */
  MPI_Finalize();
  return EXIT_SUCCESS;
}

