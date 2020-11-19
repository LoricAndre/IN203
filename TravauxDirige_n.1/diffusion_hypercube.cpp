#include <cstdlib>
#include <sstream>
#include <string>
#include <fstream>
#include <iostream>
#include <iomanip>
#include <mpi.h>
#include <cmath>

// Reconstruct rank from coords in dimension
int rank_from_coords(int* coords, int dimension) {
  int rank = 0;
  int multiplier = 1;
  for (int i = 0; i < dimension; i++) {
    rank += coords[i]*multiplier;
    multiplier *= 2;
  }
  return rank;
}

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
  // Fist, we check that the thread is actually an hypercube node.
  // Compute dimension from nbp
  int dimension = log2(nbp);
  if (rank >= pow(2, dimension)) {
    MPI_Finalize();
    return EXIT_SUCCESS;
  }
  /* File */
  std::stringstream fileName;
  fileName << "hypercube" << std::setfill('0') << std::setw(5) << rank << ".txt";
  std::ofstream output( fileName.str().c_str() );

  output << "Working in D" << dimension << std::endl;
  
  // Compute coords from rank
  int coords[dimension];
  int a = rank;
  for (int i = 0; i < dimension; i++) {
    coords[i] = a % 2;
    a /= 2;
    output << coords[i] << std::endl;
  }
  // We listen to any of the previous points (adjacents with all coords <=)
  int prev;
  if (rank == 0) {
    prev = 0;
  } else {
    MPI_Request reqs[dimension];
    for (int i = 0; i < dimension; i++) {
      if (coords[i] == 1) {
        // we reconstruct the rank from coords
        int prev_coords[dimension];
        std::copy(coords, coords + dimension, prev_coords);
        prev_coords[i] = 0;
        int prev_rank = rank_from_coords(prev_coords, dimension);
        MPI_Irecv(&prev, 1, MPI_INT, prev_rank, 0, MPI_COMM_WORLD, &reqs[i]);
      } else {
        // This is not actually supposed to end
        MPI_Irecv(&prev, 1,  MPI_INT, 0, 0, MPI_COMM_WORLD, &reqs[i]);
      }
    }
    int received_from;
    MPI_Waitany(dimension, reqs, &received_from, &status);
  }

  output << "Index got is " << prev << std::endl;
  
  //
  // We send it back
  // Compute the number of children
  int children = 0;
  for (int i = 0; i < dimension; i++) {
    if (coords[i] == 0) {
      children++;
    }
  }

  MPI_Request reqs[children];
  int reqnb = 0;
  for (int i = 0; i < dimension; i++) {
    if (coords[i] == 0) {
      int next_coords[dimension];
      std::copy(coords, coords + dimension, next_coords);
      next_coords[i]++;
      int next_rank = rank_from_coords(next_coords, dimension);
      MPI_Isend(&prev, 1, MPI_INT, next_rank, 0, MPI_COMM_WORLD, &reqs[reqnb]);
      reqnb++;
    }
  }
  MPI_Waitall(children, reqs, &status);
  

  /* Exit properly */
  output.close();
  MPI_Finalize();
  return EXIT_SUCCESS;
}
