#include <cstdlib>
#include <sstream>
#include <string>
#include <fstream>
#include <iostream>
#include <iomanip>
#include <mpi.h>
#include <vector>
#include <iterator>
#include <cmath>

int main( int nargs, char* argv[] ) {

  MPI_Init( &nargs, &argv );
  MPI_Comm globComm;
  MPI_Comm_dup(MPI_COMM_WORLD, &globComm);
  int nbp;
  MPI_Comm_size(globComm, &nbp);
  int rank;
  MPI_Comm_rank(globComm, &rank);

  const int W = 800;
  const int H = 600;
  std::vector<int> set(W*H);
  std::vector<int>::iterator pixel = set.begin();
  // 0 is the master
  MPI_Request reqs[nbp-1];
  if (rank == 0) {
    for (int i = 0; i < nbp-1; i++) {
      MPI_Irecv(pixel + W*(i*H/(nbp-1)), H/(nbp-1)*W, MPI_INT, i+1, 0, MPI_COMM_WORLD, reqs[i]);
    }
  }
  MPI_Waitall(nbp-1, reqs, MPI_STATUSES_IGNORE);

  MPI_Finalize();
  return EXIT_SUCCESS;
}
