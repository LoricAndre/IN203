#include <chrono>
#include <random>
#include <cstdlib>
#include <sstream>
#include <string>
#include <fstream>
#include <iostream>
#include <iomanip>
#include <mpi.h>

// Attention , ne marche qu'en C++ 11 ou supérieur :
double approximate_pi(unsigned long nbSamples) {
  typedef std::chrono::high_resolution_clock myclock;
  myclock::time_point beginning = myclock::now();
  myclock::duration d = beginning.time_since_epoch();
  unsigned seed = d.count();
  std::default_random_engine generator(seed);
  std::uniform_real_distribution <double> distribution (-1.0 ,1.0);
  unsigned long nbDarts = 0;
  // Throw nbSamples darts in the unit square [-1 :1] x [-1 :1]
  for (unsigned sample = 0; sample < nbSamples; ++ sample) {
    double x = distribution(generator);
    double y = distribution(generator);
    // Test if the dart is in the unit disk
    if (x*x+y*y<=1) nbDarts ++;
  }
  // Number of nbDarts throwed in the unit disk
  double ratio = double(nbDarts)/double(nbSamples);
  return 4*ratio;
}

int main( int nargs, char* argv[] ) {
  /* Init */
  MPI_Init(&nargs, &argv);
  MPI_Comm globComm;
  MPI_Comm_dup(MPI_COMM_WORLD, &globComm);
  int nbp;
  MPI_Comm_size(globComm, &nbp);
  int rank;
  MPI_Comm_rank(globComm, &rank);

  /* Tasks : */
  if (rank == 0) {
  /* Thread 0 will do the aggregation */
    double res;
    double total = 0;
    for (int i = 1; i < nbp; i++) {
      MPI_Irecv(&res, 1, MPI_DOUBLE, i, 0, MPI_COMM_WORLD);
      total += res;
    }
    total /= (nbp - 1);
    std::cout << total << std::endl;
  } else {
  /* Other threads do the work */
    double res = approximate_pi(10000000);
    MPI_Send(&res, 1, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD);
  }

  MPI_Finalize();
  return EXIT_SUCCESS;
}
