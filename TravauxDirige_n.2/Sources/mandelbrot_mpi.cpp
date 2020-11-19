# include <iostream>
# include <cstdlib>
# include <mpi.h>
# include <vector>
#include <iterator>
# include "Mandelbrot.cpp"
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
  const int maxIter = 16777216;
  std::vector<int> set(W*H);
  int pixels_per_process = H * W / (nbp - 1);
  // 0 is the master
  MPI_Request reqs[nbp-1];
  if (rank == 0) {
    for (int i = 0; i < nbp-1; i++) {
      MPI_Send(&maxIter, 1, MPI_INT, i+1, 0, MPI_COMM_WORLD);
      MPI_Irecv(set.data()+i*pixels_per_process, pixels_per_process, MPI_INT, i+1, 0, MPI_COMM_WORLD, &reqs[i]);
    }
    MPI_Waitall(nbp-1, reqs, MPI_STATUSES_IGNORE);

    savePicture("mandelbrot.tga", W, H, set, maxIter);
  } else {
    int maxIter;
    MPI_Recv(&maxIter, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    int lines = H / (nbp-1);
    std::vector<int> pixels(H*W/(nbp-1));
    for (int i = 0; i < lines; i++) {
      computeMandelbrotSetRow(W, H, maxIter, (i+rank)*W, &(pixels[i*W]));
    }

    MPI_Send(pixels.data(), lines*W, MPI_INT, 0, 0, MPI_COMM_WORLD);
  }

  MPI_Finalize();
  return EXIT_SUCCESS;
}
