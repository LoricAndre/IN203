// Produit matrice-vecteur
# include <cassert>
# include <vector>
# include <iostream>
#include <mpi.h>

// ---------------------------------------------------------------------
class Matrix : public std::vector<double>
{
  public:
    Matrix (int dim);
    Matrix(int nrows, int ncols);
    Matrix(const Matrix& A) = delete;
    Matrix(Matrix&& A) = default;
    ~Matrix() = default;

    Matrix& operator = (const Matrix& A) = delete;
    Matrix& operator = (Matrix&& A) = default;

    double& operator () (int i, int j) {
      return m_arr_coefs[i + j*m_nrows];
    }
    double  operator () (int i, int j) const {
      return m_arr_coefs[i + j*m_nrows];
    }
    double* data();

    std::vector<double> operator * (const std::vector<double>& u) const;

    std::ostream& print(std::ostream& out) const
    {
      const Matrix& A = *this;
      out << "[\n";
      for (int i = 0; i < m_nrows; ++i) {
        out << " [ ";
        for (int j = 0; j < m_ncols; ++j) {
          out << A(i,j) << " ";
        }
        out << " ]\n";
      }
      out << "]";
      return out;
    }
  private:
    int m_nrows, m_ncols;
    std::vector<double> m_arr_coefs;
};
// ---------------------------------------------------------------------
  inline std::ostream& 
operator << (std::ostream& out, const Matrix& A){
  return A.print(out);
}
// ---------------------------------------------------------------------
  inline std::ostream&
operator << (std::ostream& out, const std::vector<double>& u){
  out << "[ ";
  for (const auto& x : u)
    out << x << " ";
  out << " ]";
  return out;
}
// ---------------------------------------------------------------------
std::vector<double> 
Matrix::operator * (const std::vector<double>& u) const
{
  const Matrix& A = *this;
  assert(u.size() == unsigned(m_ncols));
  std::vector<double> v(m_nrows, 0.);
  for (int i = 0; i < m_nrows; ++i) {
    for (int j = 0; j < m_ncols; ++j) {
      v[i] += A(i,j)*u[j];
    }            
  }
  return v;
}

// =====================================================================
Matrix::Matrix (int dim) : m_nrows(dim), m_ncols(dim),
  m_arr_coefs(dim*dim){
  for (int i = 0; i < dim; ++ i) {
    for (int j = 0; j < dim; ++j) {
      (*this)(i,j) = (i+j)%dim;
    }
  }
}
// ---------------------------------------------------------------------
Matrix::Matrix(int nrows, int ncols) : m_nrows(nrows), m_ncols(ncols),
  m_arr_coefs(nrows*ncols){
  int dim = (nrows > ncols ? nrows : ncols);
  for (int i = 0; i < nrows; ++ i) {
    for (int j = 0; j < ncols; ++j) {
      (*this)(i,j) = (i+j)%dim;
    }
  }    
}

double* Matrix::data() {
  return m_arr_coefs.data();
}

// =====================================================================
int main(int nargs, char* argv[]){
  const int N = 120;
  Matrix A(N);
  std::cout  << "A : " << A << std::endl;
  std::vector<double> u(N);
  for (int i = 0; i < N; ++i)
    u[i] = i+1;
  std::cout << " u : " << u << std::endl;
  // Start
  MPI_Init(&nargs, &argv);
  MPI_Comm globComm;
  MPI_Comm_dup(MPI_COMM_WORLD, &globComm);
  int nbp;
  MPI_Comm_size(globComm, &nbp);
  int rank;
  MPI_Comm_rank(globComm, &rank);

  const int Nloc = nbp - 1;
  if (rank == 0) {
    std::vector<double> v(N);
    for (int i = 0; i < Nloc; i++) {
      MPI_Recv(v.data()+i*N/Nloc, N/Nloc, MPI_DOUBLE, i+1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    }
    std::cout << "v: " << v << std::endl;
  } else {
    std::vector<double> v(N/Nloc);
    for (int i = 0; i < N/Nloc; i++) {
      for (int k = 0; k < N; k++) {
        v[i] += A(i, k) * u[k];
      }
    }
    MPI_Send(v.data(), N/Nloc, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD);
  }




  MPI_Finalize();
  return EXIT_SUCCESS;
}
int columns_main(int nargs, char* argv[]){
  const int N = 120;
  Matrix A(N);
  std::cout  << "A : " << A << std::endl;
  std::vector<double> u(N);
  for (int i = 0; i < N; ++i)
    u[i] = i+1;
  std::cout << " u : " << u << std::endl;
  // Start
  MPI_Init(&nargs, &argv);
  MPI_Comm globComm;
  MPI_Comm_dup(MPI_COMM_WORLD, &globComm);
  int nbp;
  MPI_Comm_size(globComm, &nbp);
  int rank;
  MPI_Comm_rank(globComm, &rank);

  const int Nloc = nbp - 1;
  if (rank == 0) {
    std::vector<double> v(N);
    for (int i = 0; i < Nloc; i++) {
      std::vector<double> this_v(N);
      MPI_Recv(this_v.data(), N, MPI_DOUBLE, i+1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
      for (int j = 0; j < N; j++)
        v[j] += this_v[j];
    }
    std::cout << "v: " << v << std::endl;
  } else {
    std::vector<double> v(N);
    for (int i = 0; i < N; i++) {
      for (int k = (rank-1)*N/Nloc; k < rank*N/Nloc; k++) {
        v[i] += A(i, k) * u[k];
      }
    }
    MPI_Send(v.data(), N, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD);
  }




  MPI_Finalize();
  return EXIT_SUCCESS;
}

int old_main( int nargs, char* argv[] )
{
    const int N = 120;
    Matrix A(N);
    std::cout  << "A : " << A << std::endl;
    std::vector<double> u( N );
    for ( int i = 0; i < N; ++i ) u[i] = i+1;
    std::cout << " u : " << u << std::endl;
    std::vector<double> v = A*u;
    std::cout << "A.u = " << v << std::endl;
    return EXIT_SUCCESS;
}
